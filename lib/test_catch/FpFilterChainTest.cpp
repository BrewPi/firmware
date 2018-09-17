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
#include "FpFilterChain.h"
#include "TestMatchers.hpp"
//#include <algorithm> // std::copy
//#include <cassert>
#include <fstream>
#include <iostream>
//#include <iterator> // std::ostream_iterator
//#include <math.h>
//#include "../inc/Temperature.h"
#include <sstream>
#include <vector>

using temp_t = safe_elastic_fixed_point<11, 12, int32_t>;

SCENARIO("Fixed point filterchain using temp_t")
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
            auto chain = FpFilterChain<temp_t>(chainSpec);

            temp_t step = 10;
            WHEN("A step input of std::to_string(step) is applied")
            {
                uint32_t count = 0;
                while (count++ < 2000) {
                    chain.add(step);
                }
                THEN("The steady state output matches the step amplitude")
                {
                    CHECK(chain.read() == Approx(temp_t(10)).epsilon(0.001));
                }
            }
        }
    }

    GIVEN("A chain of filters that is not downsampled in between")
    {
        auto chain = FpFilterChain<temp_t>({0, 0}, std::vector<uint8_t>{1, 1});

        WHEN("A step input of 100 is applied")
        {
            uint32_t count = 0;
            temp_t step = 100;
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
            CHECK_THAT(chain.read(), IsWithinOf(temp_t(0.01), temp_t(100.0)));
        }
    }

    WHEN("Some filter chains are excited with a sine wave")
    {
        auto sine = [](const uint32_t& t, const uint32_t& period, const temp_t& ampl) {
            auto result = ampl * temp_t(sin(2.0 * M_PI * t / period));
            return temp_t(result);
        };

        auto chains
            = std::vector<FpFilterChain<temp_t>>{
                FpFilterChain<temp_t>({0, 0}, std::vector<uint8_t>{2, 1}), // 28
                FpFilterChain<temp_t>({2, 0}, std::vector<uint8_t>{4, 1}), // 56
                FpFilterChain<temp_t>({2, 2, 0}, {4, 3, 1}),               // 171
                FpFilterChain<temp_t>({2, 2, 2}, {4, 3, 1}),               // 257
                FpFilterChain<temp_t>({2, 2, 2, 0}, {4, 4, 3, 1}),         // 683
                FpFilterChain<temp_t>({2, 2, 2, 2}, {4, 4, 4, 1}),         // 1343
                FpFilterChain<temp_t>({2, 2, 2, 2, 0}, {4, 4, 4, 3, 1}),   // 2729
            };

        auto findGainAtPeriod = [&sine](FpFilterChain<temp_t>& c, const uint32_t& period) {
            const temp_t amplIn = 10;
            temp_t max = 0;
            c.reset(0);
            for (uint32_t t = 0; t < period * 10; ++t) {
                auto wave = sine(t, period, amplIn);
                c.add(wave);
                auto filterOutput = c.read();
                if (t > 4 * period) { // ignore start
                    max = std::max(cnl::abs(filterOutput), max);
                }
            }
            return double(max) / amplIn;
        };

        auto findHalfAmplitudePeriod = [&findGainAtPeriod](FpFilterChain<temp_t>& c) {
            uint32_t period = 10;
            while (true) {
                auto gain = findGainAtPeriod(c, period);
                if (gain >= temp_t(0.5)) {
                    std::cout << "\n"
                              << "Period of square wave that is decreased by 0.5: "
                              << period << "\n";
                    return period;
                }
                auto periodIncreaseFactor = 1.0 + (0.5 - gain); // fast approach until close
                periodIncreaseFactor = std::clamp(periodIncreaseFactor, 1.0, 1.1);

                period = std::max(period * periodIncreaseFactor, period + 1.0); // add at least 1
                std::cout << "[" << gain << ", " << period << "]\t";
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

            CHECK(findHalfAmplitudePeriod(chains[6]) == 2730);
            CHECK(findGainAtPeriod(chains[6], 1200) < 0.1);
            CHECK(findGainAtPeriod(chains[6], 4800) > 0.8);
        }

        AND_WHEN("The step threshold is set, a slow filter will respond much quicker to a step input")
        {
            auto findStepResponseDelay = [](FpFilterChain<temp_t>& c, const temp_t& stepAmpl) {
                c.reset(0);
                uint32_t t = 0;
                uint32_t stagesFinished = 0;
                for (t = 0; t < 10000 && stagesFinished < c.length(); ++t) {
                    c.add(stepAmpl);
                    for (uint8_t i = 0; i < c.length(); ++i) {
                        auto filterOutput = c.read(i);
                        if (filterOutput >= stepAmpl / 2 && i >= stagesFinished) { // ignore start
                            std::cout << "stage " << +i << " at 50\% at t=" << t << ", derivative:" << c.readDerivative<temp_t>(i) << "\n";
                            ++stagesFinished;
                        }
                    }
                }
                std::cout << "\n";
                return t;
            };

            CHECK(findStepResponseDelay(chains[6], temp_t(100)) == 2496);
            CHECK(findStepResponseDelay(chains[6], temp_t(10)) == 2496);
            CHECK(findStepResponseDelay(chains[6], temp_t(0.9)) == 2496);
            chains[6].setStepThreshold(1);
            CHECK(findStepResponseDelay(chains[6], 100) < 400);
            CHECK(findStepResponseDelay(chains[6], 10) < 2496 / 2);
            CHECK(findStepResponseDelay(chains[6], 0.9) == 2496);
        }
    }
}
