#pragma once

#include <limits>
#include <cstdint>

#define FILTER_ORDER 6

class IirFilter {
private:
    struct FilterParams {
        // params can be stored as int32_t, because they will be promoted when multiplied with _xv and _yv
        int32_t b[FILTER_ORDER + 1]; // multiplied with _xv
        int32_t a[FILTER_ORDER + 1]; // multiplied with _yv
        uint8_t shift;               // fixed point filter parameters are shifted left by this many bits
        uint8_t downsample;          // filter is suitable for down sampling this factor.
        int32_t maxDerivative;       // max derivative on a step response of 1<<shift
    };

    int64_t xv[FILTER_ORDER + 1];
    int64_t yv[FILTER_ORDER + 1];
    uint8_t paramsIdx;
    int32_t fastStepThreshold;
    uint8_t fastStepsRemaining;

    FilterParams const& params() const;
    int64_t shift(const int64_t val) const;
    int64_t unshift(const int64_t val) const;
    int64_t shift(const int64_t val, uint8_t shift) const;
    int64_t unshift(const int64_t val, uint8_t shift) const;

public:
    IirFilter(const uint8_t& idx, const int32_t& threshold = std::numeric_limits<int32_t>::max());
    ~IirFilter();
    static FilterParams const& FilterDefinition(const uint8_t idx);
    static double const dcGain(uint8_t idx); // mainly for testing
    bool add(const int32_t val);
    bool add(const int64_t val, uint8_t fractionBits);
    void setParamsIdx(const uint8_t);
    uint8_t getParamsIdx() const;
    void setStepThreshold(const int32_t);
    int32_t getStepThreshold() const;
    int32_t read(void) const;
    int64_t readWithNFractionBits(uint8_t bits) const;
    uint8_t downsamplePeriod() const;
    uint8_t fractionBits() const
    {
        return params().shift;
    }
    int32_t readLastInput() const
    {
        return unshift(xv[0]);
    }

    struct DerivativeResult {
        int64_t result;
        uint8_t fractionBits;
    };

    DerivativeResult readDerivative() const // returns unshifted derivative
    {
        return {yv[0] - yv[1], fractionBits()};
    }

    int32_t unityStepDerivative() const
    {
        return params().maxDerivative;
    }
    void
    reset(const int32_t& value);
    void
    resetInternal(const int64_t& value);
};
