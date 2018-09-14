/*
 * Filters FS * 1/4
 *
 */
#include "../inc/IirFilter.h"

#include <stdlib.h>

IirFilter::IirFilter(const uint8_t idx, const int32_t threshold)
    : xv{0}
    , yv{0}
    , fastStepsRemaining(FILTER_ORDER + 1)
    , // this will trigger faststep on first reads to initialise the filter
    paramsIdx(idx)
{
    setStepThreshold(threshold);
}

IirFilter::~IirFilter()
{
}

void
IirFilter::setStepThreshold(const int32_t threshold)
{
    fastStepThreshold = threshold;
}
int32_t
IirFilter::getStepThreshold() const
{
    return fastStepThreshold;
}

bool
IirFilter::add(const int32_t val)
{
    return add(val, 0);
}

bool
IirFilter::add(const int64_t val, uint8_t fractionBits)
{
    int64_t output = 0;
    FilterParams const& paramsRef = params();

    // shift input/output history by 1 position
    for (uint8_t i = FILTER_ORDER; i >= 1; i--) {
        xv[i] = xv[i - 1];
        yv[i] = yv[i - 1];
    }
    xv[0] = shift(val, params().shift - fractionBits);

    output = paramsRef.b[0] * xv[0];
    for (uint8_t i = 1; i <= FILTER_ORDER; i++) {
        output += paramsRef.b[i] * xv[i]; // 19 bits max + 24 bits + 16 bits = 59 bits max
        output -= paramsRef.a[i] * yv[i]; // 19 bits max + 24 bits + 16 bits = 59 bits max
    }
    yv[0] = unshift(output); // rounded shift

    // If the output of filter is rising fast, we detect this as a step and copy the input directly to the output history
    // To prevent false triggers (not a step), we take the difference between the last 2 outputs instead of the input.
    // This provides some filtering.
    // All values of the output history are set to the new value to prevent instability

    int64_t thresholdAtOutPut = uint64_t(fastStepThreshold) * uint64_t(params().maxDerivative);
    if (abs(yv[0] - yv[1]) >= thresholdAtOutPut) {
        resetInternal(xv[0]);
        return true;
    }
    return false;
}

void
IirFilter::reset(const int32_t& value)
{
    resetInternal(shift(value));
}

void
IirFilter::resetInternal(const int64_t& value)
{
    for (uint8_t i = 0; i <= FILTER_ORDER; i++) {
        // set history to same value to prevent instability and ringing after the step
        yv[i] = value;
        xv[i] = value;
    }
}

int32_t
IirFilter::read(void) const
{
    return unshift(yv[0]);
}

int64_t
IirFilter::readWithNFractionBits(uint8_t bits) const
{
    if (bits >= params().shift) {
        return shift(yv[0], bits - params().shift);
    }
    return unshift(yv[0], params().shift - bits);
}

int64_t
IirFilter::shift(const int64_t val) const
{
    return shift(val, params().shift);
}

int64_t
IirFilter::shift(const int64_t val, uint8_t shift) const
{
    return val << shift;
}
int64_t
IirFilter::unshift(const int64_t val) const
{
    return unshift(val, params().shift);
}

int64_t
IirFilter::unshift(const int64_t val, uint8_t shift) const
{
    const int64_t rounder = 1 << (shift - 1);
    // round towards zero, which is unbiased
    // safer than away from zero, which can make the value run away from zero slowly
    int64_t rounded = (val > 0) ? val - rounder : val + rounder;
    return rounded >> shift;
}

// Try out these filters in pyFDA to view Magnitude response and stability
IirFilter::FilterParams const&
IirFilter::FilterDefinition(const uint8_t idx)
{
    static const FilterParams availableFilters[] = {
        // 0 - Bessel 6th order, -60 dB > 1/4 FS, To downsample 2x. -3dB at 0.0575 FS
        {
            {
                34,
                202,
                506,
                675,
                506,
                202,
                34,
            },
            {
                131072,
                -449749,
                672637,
                -556881,
                267670,
                -70519,
                7929,
            },
            17,
            2,
            20773,
        },
        // 1 - Bessel 6th order, -50 dB > 1/4 FS, To downsample 2x. -3dB at 0.069 FS
        {
            {
                77,
                460,
                1149,
                1531,
                1149,
                460,
                77,
            },
            {
                131072,
                -394137,
                530035,
                -401475,
                178825,
                -44094,
                4677,
            },
            17,
            2,
            24599,
        },
        // 2 - Bessel 6th order, -40 dB > 1/8 FS, To downsample 4x. Fc at 0.06125, -3dB at 0.035 FS
        {
            {
                3,
                18,
                46,
                61,
                46,
                18,
                3,
            },
            {
                131072,
                -569338,
                1045651,
                -1037968,
                586655,
                -178824,
                22947,
            },
            17,
            4,
            13073,
        },
    };

    if (idx < sizeof(availableFilters) / sizeof(availableFilters[0])) {
        return availableFilters[idx];
    } else {
        return availableFilters[0];
    }
}

IirFilter::FilterParams const&
IirFilter::params() const
{
    return FilterDefinition(paramsIdx);
}

void
IirFilter::setParamsIdx(const uint8_t idx)
{
    // shift output history to match new filter params
    int8_t shiftDifference = FilterDefinition(idx).shift - FilterDefinition(paramsIdx).shift;
    for (uint8_t i = 0; i <= FILTER_ORDER; i++) {
        yv[i] = (shiftDifference >= 0) ? yv[i] << shiftDifference : yv[i] >> (-shiftDifference);
    }
    paramsIdx = idx;
}

uint8_t
IirFilter::getParamsIdx() const
{
    return paramsIdx;
}

uint8_t
IirFilter::downsamplePeriod() const
{
    return params().downsample;
}

double const
IirFilter::dcGain(uint8_t idx)
{
    long double num = 0;
    long double den = 0;

    for (uint8_t i = 0; i <= FILTER_ORDER; i++) {
        num += FilterDefinition(idx).b[i];
        den += FilterDefinition(idx).a[i];
    }
    return num / den;
}
