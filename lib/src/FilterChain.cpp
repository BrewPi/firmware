/*
 * Copyright 2018 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FilterChain.h>
#include <memory>

FilterChain::FilterChain(
    const std::vector<uint8_t>& params,
    const std::vector<uint8_t>& intervals,
    const int32_t& stepThreshold,
    const uint8_t& thresholdStartIdx)
{
    setParams(params);
    setStepThreshold(stepThreshold);

    // interval of zero means to use the filter's downsample period, any other value overrides it
    this->intervals = std::vector<uint8_t>(params.size(), 0);
    auto it_arg = intervals.begin();
    auto it_class = this->intervals.begin();
    auto it_f = this->filters.begin();
    for (; it_class != this->intervals.end(); it_arg++, it_class++, it_f++) {
        *it_class = (it_arg < intervals.end() && *it_arg != 0) ? *it_arg : it_f->downsamplePeriod();
    }
}

FilterChain::FilterChain(
    const std::vector<uint8_t>& params,
    const int32_t& stepThreshold,
    const uint8_t& thresholdStartIdx)
    : FilterChain(params, std::vector<uint8_t>(), stepThreshold, thresholdStartIdx)
{
}

void
FilterChain::add(const int64_t val, const uint8_t fractionBits)
{
    uint32_t updatePeriod = 1;
    int64_t nextFilterIn = val;
    uint8_t nextFilterInFractionBits = fractionBits;
    auto it_f = filters.begin();
    auto it_i = intervals.begin();
    for (; it_f != filters.end(); it_f++, it_i++) {
        it_f->add(nextFilterIn, nextFilterInFractionBits);
        updatePeriod *= *it_i; // calculate how often the next filter should be updated
        if (counter % updatePeriod != updatePeriod - 1) {
            break; // only move onto next filter if it needs to be updated
        }
        nextFilterInFractionBits = it_f->fractionBits();
        nextFilterIn = it_f->readWithNFractionBits(nextFilterInFractionBits);
    }
    counter++;
    if (counter == minSampleInterval()) {
        counter = 0; // reset counter if last filter has had all its updates
    }
}

void
FilterChain::reset(const int32_t& value)
{
    for (auto& f : filters) {
        f.reset(value);
    }
}

void
FilterChain::setParams(std::vector<uint8_t> params)
{
    if (params.size() < 1) {
        return;
    }
    auto itP = params.begin();
    auto itF = filters.begin();

    // set params on already existing filters
    for (; itF != filters.end() && itP != params.end();
         itP++, itF++) {
        itF->setParamsIdx(*itP);
    }
    // append new filters
    for (; itP != params.end(); itP++) {
        filters.push_back(IirFilter(*itP, INT32_MAX));
    }
    filters.shrink_to_fit(); // remove filters if params is shorter than before

    // set step threshold again so new filters at the end also have the correct value
    setStepThreshold(getStepThreshold(0));
}

std::vector<uint8_t>
FilterChain::getParams()
{
    std::vector<uint8_t> params;
    for (auto& v : filters) {
        params.push_back(v.getParamsIdx());
    }
    return params;
}

void
FilterChain::setStepThreshold(const int32_t& threshold)
{
    int32_t adjustedThreshold = threshold;
    for (auto& f : filters) {
        f.setStepThreshold(adjustedThreshold);
        // adjustedThreshold = adjustedThreshold / 2; TODO, make sure threshold works in chain
    }
}

int32_t
FilterChain::getStepThreshold()
{
    if (filters.size() < 1) {
        return INT32_MAX;
    }
    return filters.back().getStepThreshold();
}

int32_t
FilterChain::getStepThreshold(uint8_t filterNr)
{
    if (filterNr > filters.size() - 1) {
        return filters.back().getStepThreshold();
    }
    return filters[filterNr].getStepThreshold();
}

int32_t
FilterChain::read(uint8_t filterNr)
{
    if (filterNr >= filters.size()) {
        return 0;
    }
    return filters[filterNr].read();
}

int32_t
FilterChain::read()
{
    return read(filters.size() - 1);
}

int64_t
FilterChain::readWithNFractionBits(uint8_t filterNr, uint8_t bits)
{
    if (filterNr >= filters.size()) {
        return filters.back().readWithNFractionBits(bits);
    }
    return filters[filterNr].readWithNFractionBits(bits);
}

int64_t
FilterChain::readWithNFractionBits(uint8_t bits)
{
    return filters.back().readWithNFractionBits(bits);
}

uint32_t
FilterChain::minSampleInterval(uint8_t filterNr)
{
    if (filterNr > filters.size() - 1) {
        return -1;
    }
    int32_t interval = 1;
    auto it = intervals.begin();
    for (; it != intervals.end() && it != intervals.begin() + filterNr + 1; it++) {
        interval *= *it;
    }
    return interval;
}

uint32_t
FilterChain::minSampleInterval()
{
    return minSampleInterval(filters.size() - 1);
}

uint8_t
FilterChain::fractionBits(uint8_t filterNr)
{
    if (filterNr >= filters.size()) {
        return filters.back().fractionBits();
    }
    return filters[filterNr].fractionBits();
}

uint8_t
FilterChain::fractionBits()
{
    return filters.back().fractionBits();
}

int32_t
FilterChain::readLastInput()
{
    return filters.front().readLastInput();
}

int32_t
FilterChain::readDerivative(uint8_t filterNr, int32_t scale)
{
    if (filterNr > filters.size() - 1) {
        return 0;
    }
    return filters[filterNr].readDerivative(scale);
}
