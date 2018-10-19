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

#include "catch.hpp"

#include "../inc/future_std.h"
#include "FilterChain.h"
#include "TestMatchers.hpp"
#include <algorithm> // std::copy
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator> // std::ostream_iterator
#include <math.h>
#include <sstream>
#include <vector>

SCENARIO("Basic test of chain of filters")
{
    std::vector<std::vector<uint8_t>> chainsSpecs = {{0},
                                                     {0, 1},
                                                     {0, 1, 1}};

    for (auto& chainSpec : chainsSpecs) {
        std::stringstream chain_str;
        std::ostream_iterator<uint8_t> out_it(chain_str, ", ");
        std::copy(chainSpec.begin(), chainSpec.end(), out_it);

        GIVEN("A filter chain of filters: " + chain_str.str())
        {
            FilterChain chain(chainSpec);

            int32_t step = 5000;
            WHEN("A step input of std::to_string(step) is applied")
            {
                uint32_t count = 0;
                while (count++ < 2000) {
                    chain.add(step);
                }
                THEN("The steady state output of the chain is " + std::to_string(step))
                {
                    CHECK_THAT(chain.read(), IsWithinOf(int32_t(chainSpec.size()), step)); // 1 bit difference allowed for each filter in the chain
                }
            }
        }
    }

    GIVEN("A chain of filters that is not downsampled in between")
    {
        FilterChain chain({0, 0}, {1, 1});

        WHEN("A step input of 100000 is applied")
        {
            uint32_t count = 0;
            int32_t step = 100000;
            //char csv[] = "../test-results/test.csv";
            ///std::ofstream csvFile(csv);
            while (count++ < 1000) {
                chain.add(step);
                /*				csvFile << count;
				for(int i = 0; i < chain.length(); i++){
					csvFile << "," << chain.read(i);
				}
				csvFile << std::endl;
*/
            }
            //csvFile.close();
            CHECK_THAT(chain.read(), IsWithinOf(1, 100000));
        }
    }

    WHEN("Some filter chains are excited with a sine wave")
    {
        auto sine = [](const uint32_t& t, const uint32_t& period, const int32_t& ampl) {
            return ampl * sin(2.0 * M_PI * t / period);
        };

        auto chains
            = std::vector<FilterChain>{
                FilterChain({0, 0}, {2, 1}),                   // 28
                FilterChain({2, 0}, {4, 1}),                   // 56
                FilterChain({2, 2, 0}, {4, 3, 1}),             // 171
                FilterChain({2, 2, 2}, {4, 3, 1}),             // 257
                FilterChain({2, 2, 2, 0}, {4, 4, 3, 1}),       // 683
                FilterChain({2, 2, 2, 2}, {4, 4, 4, 1}),       // 1343
                FilterChain({2, 2, 2, 2, 0}, {4, 4, 4, 3, 1}), // 2729
            };

        auto findGainAtPeriod = [&sine](FilterChain& c, const uint32_t& period) {
            int32_t amplIn = 100000;
            int32_t max = 0;
            c.reset(0);
            for (uint32_t t = 0; t < period * 10; ++t) {
                int32_t wave = sine(t, period, amplIn);
                c.add(wave);
                int32_t filterOutput = c.read();
                if (t > 4 * period) { // ignore start
                    max = std::max(std::abs(filterOutput), max);
                }
            }
            return double(max) / amplIn;
        };

        auto findHalfAmplitudePeriod = [&findGainAtPeriod](FilterChain& c) {
            uint32_t period = 10;
            while (true) {
                auto gain = findGainAtPeriod(c, period);
                if (gain > 0.5) {
                    // std::cout << "\n"
                    //           << "Period of square wave that is decreased by 0.5: "
                    //           << period << "\n";
                    return period;
                }
                auto periodIncreaseFactor = 1.0 + (0.5 - gain); // fast approach until close
                periodIncreaseFactor = std::clamp(periodIncreaseFactor, 1.0, 1.1);

                period = std::max(period * periodIncreaseFactor, period + 1.0); // add at least 1
                // std::cout << "[" << gain << ", " << period << "]\t";
            };
        };

        THEN("They block higher frequencies and pass lower frequencies")
        {
            CHECK(findHalfAmplitudePeriod(chains[0]) == 28);
            CHECK(findGainAtPeriod(chains[0], 14) < 0.1);
            CHECK(findGainAtPeriod(chains[0], 56) > 0.8);

            CHECK(findHalfAmplitudePeriod(chains[1]) == 55);
            CHECK(findGainAtPeriod(chains[1], 28) < 0.1);
            CHECK(findGainAtPeriod(chains[1], 120) > 0.8);

            CHECK(findHalfAmplitudePeriod(chains[2]) == 171);
            CHECK(findGainAtPeriod(chains[2], 80) < 0.1);
            CHECK(findGainAtPeriod(chains[2], 300) > 0.8);

            CHECK(findHalfAmplitudePeriod(chains[3]) == 257);
            CHECK(findGainAtPeriod(chains[3], 120) < 0.1);
            CHECK(findGainAtPeriod(chains[3], 500) > 0.8);

            CHECK(findHalfAmplitudePeriod(chains[4]) == 683);
            CHECK(findGainAtPeriod(chains[4], 300) < 0.1);
            CHECK(findGainAtPeriod(chains[4], 1200) > 0.8);

            CHECK(findHalfAmplitudePeriod(chains[5]) == 1343);
            CHECK(findGainAtPeriod(chains[5], 600) < 0.1);
            CHECK(findGainAtPeriod(chains[5], 2400) > 0.8);

            CHECK(findHalfAmplitudePeriod(chains[6]) == 2729);
            CHECK(findGainAtPeriod(chains[6], 1200) < 0.1);
            CHECK(findGainAtPeriod(chains[6], 4800) > 0.8);
        }

        AND_WHEN("The step threshold is set, a slow filter will respond much quicker to a step input")
        {
            auto findStepResponseDelay = [](FilterChain& c, const int32_t& stepAmpl) {
                c.reset(0);
                uint32_t t = 0;
                uint32_t stagesFinished = 0;
                for (t = 0; t < 10000 && stagesFinished < c.length(); ++t) {
                    c.add(stepAmpl);
                    for (uint8_t i = 0; i < c.length(); ++i) {
                        int32_t filterOutput = c.read(i);
                        if (filterOutput >= stepAmpl / 2 && i >= stagesFinished) { // ignore start
                            // std::cout << "stage " << +i << " at 50\% at t=" << t << "\t\t";
                            ++stagesFinished;
                        }
                    }
                }
                // std::cout << "\n";
                return t;
            };

            CHECK(findStepResponseDelay(chains[6], 100000) == 2496);
            CHECK(findStepResponseDelay(chains[6], 900) == 2496);
            chains[6].setStepThreshold(1000);
            CHECK(findStepResponseDelay(chains[6], 100000) < 400);
            CHECK(findStepResponseDelay(chains[6], 900) == 2496);
        }
    }
}

void
test_frequencies(
    FilterChain& chain,
    std::vector<double> freq,
    std::vector<double> ampl,
    double input_freq,
    double max_output_min,
    double max_output_max,
    char* csv = nullptr)
{

    double t = 0;
    double dt = double(1.0) / double(input_freq);
    double max_period = 1.0 / *std::min_element(freq.begin(), freq.end());
    double t_end = std::max(10.0 * max_period, 10.0 * chain.minSampleInterval() * dt);
    double sensor = 0;
    double max = 0;
    double t_max = 0;

    std::ofstream csvFile(csv);

    for (t = 0; t < t_end; t += dt) {
        REQUIRE(freq.size() == ampl.size());
        sensor = 0.0;
        auto f = freq.begin();
        auto a = ampl.begin();
        for (; f < freq.end(); f++, a++) {
            sensor += *a * sin(*f * 2.0 * M_PI * t);
        }
        int32_t sensor_int = sensor;
        chain.add(sensor_int);
        int32_t output = chain.read();
        double abs_output = std::abs(output);
        if (t > 0.7 * t_end && abs_output > max) {
            max = abs_output;
            t_max = t;
        }
        csvFile << t << "," << sensor;

        for (int i = 0; i < chain.length(); i++) {
            csvFile << "," << chain.read(i);
        }
        csvFile << std::endl;
    }
    csvFile.close();

    CAPTURE(t_max);
    CAPTURE(freq);
    CHECK(max >= max_output_min);
    CHECK(max <= max_output_max);
}

void
countSameValueAtOutPut(FilterChain& chain, uint32_t sameSampleCount, uint32_t counterMax)
{
    std::vector<int32_t> out;
    uint32_t counterMaxSeen = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        chain.add(100000);
        out.push_back(chain.read());
        counterMaxSeen = std::max(counterMaxSeen, chain.getCount());
    }
    uint32_t count = 1;
    auto v = out.begin() + 1;
    // skip first values to skip start where filter is near zero
    for (; *v == *(v - 1) && v != out.end(); v++) {
    }
    for (v++; *v == *(v - 1) && v != out.end(); v++) {
    }
    for (v++; *v == *(v - 1) && v != out.end(); v++) {
    }
    for (v++; *v == *(v - 1) && v != out.end(); v++) {
        count++;
    };
    std::vector<int32_t> relevantOutput = std::vector<int32_t>(out.begin(), v);
    CAPTURE(relevantOutput);
    CHECK(count == sameSampleCount);
    CHECK(counterMaxSeen == counterMax);
}

bool
isUpdatedAtCounts(FilterChain& chain, uint8_t filterIndex, std::vector<uint32_t> counts)
{
    std::vector<uint32_t> ticks;
    int32_t step = 100000;
    uint32_t count = 0;
    while (chain.getCount() != 0 || chain.read(filterIndex) < step / 2) {
        chain.add(step); // ensure the filter output is a rising slope (skip the flat start)
    }
    while (ticks.size() < counts.size() && count < 10000) {
        int32_t previousFilterVal = chain.read(filterIndex);
        chain.add(step);
        int32_t filterVal = chain.read(filterIndex);
        if (filterVal != previousFilterVal) {
            ticks.push_back(count);
        }
        ++count;
    }
    while (chain.getCount() != 0) {
        chain.add(step); // exit with count at zero again, to not offset next test
    }

    CHECK(ticks == counts);
    return ticks == counts;
}

SCENARIO("Filters in chain are ran at specified intervals", "[filterchain][intervals]")
{

    WHEN("The sample rates are used from the filter spec")
    {
        THEN("the output value changes value every [product of the intervals] samples")
        {
            FilterChain chain1({0, 2, 0, 2});
            countSameValueAtOutPut(chain1, 16, 63); // last filter updates every 16 counts, cycle is 64 counts
            FilterChain chain2({2, 2, 2});
            countSameValueAtOutPut(chain2, 16, 63);
        }
    }

    WHEN("The sample rates are specified")
    {
        THEN("the output value changes value every [product of the intervals] samples")
        {
            FilterChain chain1({2, 2, 2, 2, 2, 2}, {1, 1, 1, 1, 1, 1});
            countSameValueAtOutPut(chain1, 1, 0);
            CHECK(chain1.minSampleInterval() == 1);

            FilterChain chain2({2, 2, 2, 2, 2, 2}, {2, 1, 2, 1, 1, 4});
            countSameValueAtOutPut(chain2, 4, 15);
            CHECK(chain2.minSampleInterval() == 16);

            FilterChain chain3({2, 2, 2, 2, 2, 2}, {2, 2, 2, 1, 1, 4});
            countSameValueAtOutPut(chain3, 8, 31);
            CHECK(chain3.minSampleInterval() == 32);

            FilterChain chain4({2, 2, 2, 2, 2, 2}, {4, 2, 1, 4, 2, 1});
            countSameValueAtOutPut(chain4, 64, 63);
            CHECK(chain4.minSampleInterval() == 64);
        }
    }
    WHEN("The sample rates are specified")
    {
        THEN("the output value changes value at the expected count for each subfilter")
        {
            {
                FilterChain chain({2, 2}, {0, 0});
                CHECK(isUpdatedAtCounts(chain, 0, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 1, std::vector<uint32_t>({3, 7, 11, 15, 19})));
            }
            {
                FilterChain chain({2, 2}, {1, 0});
                CHECK(isUpdatedAtCounts(chain, 0, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 1, std::vector<uint32_t>({0, 1, 2, 3})));
            }
            {
                FilterChain chain({2, 2, 2, 2, 2, 2}, {1, 1, 1, 1, 1, 1});
                CHECK(isUpdatedAtCounts(chain, 0, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 1, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 2, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 3, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 4, std::vector<uint32_t>({0, 1, 2, 3})));
            }
            {
                FilterChain chain({2, 2, 2, 2, 2, 2}, {2, 1, 2, 1, 1, 4});
                CHECK(isUpdatedAtCounts(chain, 0, std::vector<uint32_t>({0, 1, 2, 3})));
                CHECK(isUpdatedAtCounts(chain, 1, std::vector<uint32_t>({1, 3, 5, 7})));
                CHECK(isUpdatedAtCounts(chain, 2, std::vector<uint32_t>({1, 3, 5, 7})));
                CHECK(isUpdatedAtCounts(chain, 3, std::vector<uint32_t>({3, 7, 11, 15})));
                CHECK(isUpdatedAtCounts(chain, 4, std::vector<uint32_t>({3, 7, 11, 15})));
                CHECK(isUpdatedAtCounts(chain, 5, std::vector<uint32_t>({3, 7, 11, 15})));
            }
        }
    }
}

SCENARIO("Filters chain output matches manually cascaded filters", "[filterchain][match]")
{
    WHEN("A filter chain is set up the same way as 2 normal filters after each other")
    {
        THEN("the output is the same if separate filters are read with max precision")
        {
            FilterChain chain({0, 0, 0}, {1, 1, 1});
            IirFilter f1(0);
            IirFilter f2(0);
            IirFilter f3(0);

            for (uint32_t i = 0; i < 1000; i++) {
                uint32_t v = 100000;
                chain.add(v);
                f1.add(v);
                f2.add(f1.readWithNFractionBits(f1.fractionBits()), f1.fractionBits());
                f3.add(f2.readWithNFractionBits(f2.fractionBits()), f2.fractionBits());
                CAPTURE(i);
                REQUIRE(chain.read() == f3.read());
            }
        }
        THEN("the output is almost same if separate filters are read with normal precision")
        {
            FilterChain chain({0, 0, 0}, {1, 1, 1});
            IirFilter f1(0);
            IirFilter f2(0);
            IirFilter f3(0);

            for (uint32_t i = 0; i < 1000; i++) {
                uint32_t v = 100000;
                chain.add(v);
                f1.add(v);
                f2.add(f1.read());
                f3.add(f2.read());
                CAPTURE(i);
                REQUIRE_THAT(chain.read(), IsWithinOf(50, f3.read()));
            }
        }
    }
}
