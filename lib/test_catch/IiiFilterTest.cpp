/*
 * Copyright 2018 BrewPi
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

#include <catch.hpp>

#include "../inc/FilterChain.h"
#include "TestMatchers.hpp"
#include <sstream>

SCENARIO("Filtering 24-bit values with IIR Filters", "[filter]")
{
    for (uint8_t i = 0; i <= 1; i++) {
        CAPTURE(std::to_string(i));
        CHECK(IirFilter::dcGain(i) == 1.0); // DC gain of all filters is 1

        GIVEN("IIR filter number " + std::to_string(i) + ", down-sample factor " + std::to_string(IirFilter::FilterDefinition(i).downsample))
        {

            const int numSamples = 1000;
            WHEN("The step detection is disabled and the input is 2^shift")
            {
                if (!(IirFilter::FilterDefinition(i).maxDerivative == INT32_MAX)) { // only test filters that have step detection
                    const int32_t stepValue = 1 << IirFilter::FilterDefinition(i).shift;
                    IirFilter filter(i, INT32_MAX);
                    int32_t output;
                    int32_t maxDerivative = 0;
                    THEN("the maximum derivative matches the value for max derivative in params()")
                    {
                        for (int i = 0; i < numSamples; i++) {
                            int32_t previous = filter.read();
                            filter.add(stepValue);
                            output = filter.read();
                            int32_t derivative = output - previous;
                            maxDerivative = (derivative > maxDerivative) ? derivative : maxDerivative;
                        }
                        CHECK(maxDerivative == IirFilter::FilterDefinition(i).maxDerivative);
                    }
                }
            }

            WHEN("Fast step threshold is set to 5000")
            {
                THEN("The step detection WILL NOT trigger for step of 4999")
                {
                    IirFilter filter(i, 5000);
                    IirFilter filterRef(i, INT32_MAX); // reference without step detect
                    const int32_t stepValue = 4999;
                    int triggeredAt = -1;
                    for (int i = 0; i < numSamples; i++) {
                        filter.add(stepValue);
                        filterRef.add(stepValue);
                        if (filter.read() != filterRef.read()) {
                            triggeredAt = i;
                        }
                    }
                    CHECK(triggeredAt == -1);
                }
                if (!(IirFilter::FilterDefinition(i).maxDerivative == INT32_MAX)) { // only test filters that have step detection
                    THEN("The step detection WILL trigger for step 5001")
                    {
                        IirFilter filter(i, 5000);
                        IirFilter filterRef(i, INT32_MAX); // reference without step detect
                        const int32_t stepValue = 5001;
                        int triggeredAt = -1;
                        int32_t output;
                        int32_t reference;
                        int32_t max = INT32_MIN;
                        int32_t maxSample = 0;
                        for (int i = 0; i < numSamples; i++) {
                            filter.add(stepValue);
                            filterRef.add(stepValue);
                            output = filter.read();
                            reference = filterRef.read();
                            if (output != reference) {
                                if (triggeredAt == -1) {
                                    triggeredAt = i;
                                }
                            }
                            if (output > max) {
                                max = output;
                                maxSample = i;
                            }
                        }
                        CHECK(triggeredAt != -1);
                        AND_THEN("Maximum value will be when step detection happens")
                        {
                            CHECK(triggeredAt == maxSample);
                        }
                        AND_THEN("The maximum output value is the input step value (no overshoot)")
                        {
                            CHECK_THAT(max, IsWithinOf(int32_t(1), stepValue));
                        }
                    }
                }

                if (!(IirFilter::FilterDefinition(i).maxDerivative == INT32_MAX)) { // only test filters that have step detection
                    THEN("The step response detect WILL trigger within 5 samples for a step of 50000")
                    {
                        IirFilter filter(i, 5000);
                        IirFilter filterRef(i, INT32_MAX); // reference without step detect
                        const int32_t stepValue = 50000;
                        int triggeredAt = -1;
                        int32_t output;
                        int32_t reference;
                        int32_t max = INT32_MIN;
                        int32_t maxSample = 0;
                        for (int i = 0; i < numSamples; i++) {
                            filter.add(stepValue);
                            filterRef.add(stepValue);
                            output = filter.read();
                            reference = filterRef.read();
                            if (output != reference) {
                                if (triggeredAt == -1) {
                                    triggeredAt = i;
                                }
                            }
                            if (output > max) {
                                max = output;
                                maxSample = i;
                            }
                        }
                        CHECK(triggeredAt != -1);
                        CHECK(triggeredAt <= 5);
                        AND_THEN("Maximum value will be reached within the 5 samples")
                        {
                            CHECK(maxSample <= 5);
                        }
                        AND_THEN("The maximum output value is within 2% of the input step value")
                        {
                            CHECK_THAT(max, IsWithinPercentageOf(2, stepValue));
                        }
                        AND_THEN("The final output value is within +/- 1 of the input step value")
                        {
                            CHECK_THAT(output, IsWithinOf(int32_t(1), stepValue));
                        }
                    }
                }

                THEN("The step response WILL NOT trigger on an impulse of 10000")
                {
                    IirFilter filter(i, 5000);
                    IirFilter filterRef(i, INT32_MAX); // reference without step detect
                    const int32_t spikeValue = 10000;
                    int triggeredAt = -1;
                    int32_t output;
                    int32_t reference;
                    for (int i = 0; i < numSamples; i++) {
                        if (i == 5) {
                            filter.add(spikeValue);
                            filterRef.add(spikeValue);
                        } else {
                            filter.add(0);
                            filterRef.add(0);
                        }
                        output = filter.read();
                        reference = filterRef.read();
                        if (output != reference) {
                            if (triggeredAt == -1) {
                                triggeredAt = i;
                            }
                        }
                    }
                    CHECK(triggeredAt == -1);
                }
            }

            WHEN("The input is the maximum 24 bit value, 0x00FFFFFF")
            {
                IirFilter filter(i, INT32_MAX);
                int32_t input = 0x00FFFFFF;
                THEN("the steady state equals the input")
                {
                    for (int i = 0; i < numSamples; i++) {
                        filter.add(input);
                    }
                    CHECK_THAT(filter.read(), IsWithinOf(int32_t(10), input));
                }
            }

            WHEN("The input is the minimum 24 bit value, 0xFF000000")
            {
                IirFilter filter(i, INT32_MAX);
                int32_t input = 0xFF000000;
                THEN("the steady state equals the input")
                {
                    for (int i = 0; i < numSamples; i++) {
                        filter.add(input);
                    }
                    CHECK_THAT(filter.read(), IsWithinOf(int32_t(10), input));
                }
            }

            WHEN("The input value is given with a specified number of fraction bits")
            {
                IirFilter filter(i, INT32_MAX);
                int32_t input = int32_t(1000);
                int64_t shifted = input << 14;

                THEN("the steady state equals the input")
                {
                    for (int i = 0; i < numSamples; i++) {
                        filter.add(shifted, 14);
                    }
                    CHECK_THAT(filter.read(), IsWithinOf(int32_t(1), input));
                    AND_WHEN("the output is read with a specified number of bits")
                    {
                        int64_t n_bits = filter.fractionBits();
                        int64_t no_shift = filter.readWithNFractionBits(n_bits);
                        int64_t out = filter.readWithNFractionBits(14);
                        int64_t out2 = filter.readWithNFractionBits(12);
                        int64_t out3 = filter.readWithNFractionBits(19);
                        THEN("The returned value is correct")
                        {

                            CHECK_THAT(out, IsWithinOf(int64_t(1) << (n_bits - 14), no_shift >> (n_bits - 14)));
                            CHECK_THAT(out2, IsWithinOf(int64_t(1) << (n_bits - 12), no_shift >> (n_bits - 12)));
                            CHECK_THAT(out3, IsWithinOf(int64_t(1) << (19 - n_bits), no_shift << (19 - n_bits)));
                        }
                    }
                }
            }
        }
    }
}
