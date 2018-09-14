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
#include <limits>
#include <memory>

FilterChain::FilterChain(
    const std::vector<uint8_t>& params,
    const std::vector<uint8_t>& intervals,
    const int32_t& stepThreshold)
{
    setParams(params, intervals, stepThreshold);
}

FilterChain::FilterChain(
    const std::vector<uint8_t>& params,
    const int32_t& stepThreshold)
    : FilterChain(params, std::vector<uint8_t>(), stepThreshold)
{
}

void
FilterChain::add(const int64_t val, const uint8_t fractionBits)
{
    uint32_t updatePeriod = 1;
    int64_t nextFilterIn = val;
    uint8_t nextFilterInFractionBits = fractionBits;
    for (auto& s : stages) {
        s.filter.add(nextFilterIn, nextFilterInFractionBits);
        updatePeriod *= s.interval; // calculate how often the next filter should be updated
        if (counter % updatePeriod != updatePeriod - 1) {
            break; // only move onto next filter if it needs to be updated
        }
        nextFilterInFractionBits = s.filter.fractionBits();
        nextFilterIn = s.filter.readWithNFractionBits(nextFilterInFractionBits);
    }
    counter++;
    if (counter == minSampleInterval()) {
        counter = 0; // reset counter if last filter has had all its updates
    }
}

void
FilterChain::reset(const int32_t& value)
{
    for (auto& s : stages) {
        s.filter.reset(value);
    }
}

void
FilterChain::setParams(const std::vector<uint8_t>& params, const std::vector<uint8_t>& intervals, const int32_t& stepThreshold)
{
    if (params.size() < 1) {
        return;
    }
    auto itP = params.begin();
    auto itS = stages.begin();
    auto itI = intervals.begin();

    // reconfigure already existing filters
    for (; itS != stages.end() && itP != params.end(); ++itP, ++itS) {
        if (*itP != itS->filter.getParamsIdx()) {
            itS->filter.setParamsIdx(*itP);
        }
        itS->filter.setStepThreshold(stepThreshold);

        if (itI != intervals.end()) {
            itS->interval = *itI;
            ++itI;
        } else {
            itS->interval = IirFilter::FilterDefinition(*itP).downsample;
        }
    }
    // append new filters
    for (; itP != params.end(); ++itP) {
        uint8_t interval;
        if (itI != intervals.end() && *itI != 0) {
            interval = *itI;
            ++itI;
        } else {
            interval = IirFilter::FilterDefinition(*itP).downsample;
        }

        stages.emplace_back(Stage{IirFilter(*itP, stepThreshold), std::move(interval)});
    }
    stages.shrink_to_fit(); // remove filters if params is shorter than before
}

void
FilterChain::setStepThreshold(const int32_t& threshold)
{
    int32_t adjustedThreshold = threshold;
    for (auto& s : stages) {
        s.filter.setStepThreshold(adjustedThreshold);
    }
}

int32_t
FilterChain::getStepThreshold()
{
    if (stages.size() < 1) {
        return std::numeric_limits<int32_t>::max();
    }
    return stages.front().filter.getStepThreshold();
}

int32_t
FilterChain::read(uint8_t filterNr)
{
    if (filterNr >= stages.size()) {
        return 0;
    }
    return stages[filterNr].filter.read();
}

int32_t
FilterChain::read()
{
    return read(stages.size() - 1);
}

int64_t
FilterChain::readWithNFractionBits(uint8_t filterNr, uint8_t bits)
{
    if (filterNr >= stages.size()) {
        return 0;
    }
    return stages[filterNr].filter.readWithNFractionBits(bits);
}

int64_t
FilterChain::readWithNFractionBits(uint8_t bits)
{
    return readWithNFractionBits(stages.size() - 1);
}

uint32_t
FilterChain::minSampleInterval(uint8_t filterNr)
{
    if (filterNr > stages.size() - 1) {
        return 1;
    }
    int32_t interval = 1;
    auto it = stages.begin();
    for (; it != stages.end() && it != stages.begin() + filterNr + 1; it++) {
        interval *= it->interval;
    }
    return interval;
}

uint32_t
FilterChain::minSampleInterval()
{
    return minSampleInterval(stages.size() - 1);
}

uint8_t
FilterChain::fractionBits(uint8_t filterNr)
{
    if (filterNr >= stages.size()) {
        return stages.back().filter.fractionBits();
    }
    return stages[filterNr].filter.fractionBits();
}

uint8_t
FilterChain::fractionBits()
{
    return fractionBits(stages.size() - 1);
}

int32_t
FilterChain::readLastInput()
{
    return stages.front().filter.readLastInput();
}

int32_t
FilterChain::readDerivative(uint8_t filterNr, int32_t scale)
{
    if (filterNr > stages.size() - 1) {
        return 0;
    }
    return stages[filterNr].filter.readDerivative(scale);
}
