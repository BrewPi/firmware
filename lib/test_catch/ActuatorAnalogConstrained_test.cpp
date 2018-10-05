/*
 * Copyright 2018 BrewPi B.V.
 *
 * This file is part of the BrewBlox Control Library.
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

#include "ActuatorAnalogConstrained.h"
#include "ActuatorAnalogMock.h"

SCENARIO("ActuatorAnalogConstrained test")
{
    WHEN("An actuator is constructed wrapped by a ActuatorAnalogConstrained")
    {
        auto act = ActuatorAnalogMock();
        auto cAct = ActuatorAnalogConstrained(act);

        CHECK(cAct.setting() == 0);
        CHECK(cAct.value() == 0);
        CHECK(cAct.valid() == true);

        cAct.setting(50);

        CHECK(act.setting() == 50);
        CHECK(act.value() == 50);
        CHECK(act.valid() == true);

        WHEN("A maximum constraint is added, the value is clipped at the maximum")
        {
            cAct.addConstraint(AAConstraints::Maximum(25));

            cAct.setting(50);
            CHECK(cAct.setting() == ActuatorAnalog::value_t(25));
            CHECK(act.setting() == ActuatorAnalog::value_t(25));

            AND_WHEN("A minimum constraint is also added, the value is clipped at minimum and maximum")
            {
                cAct.addConstraint(AAConstraints::Minimum(15));

                cAct.setting(50);
                CHECK(cAct.setting() == ActuatorAnalog::value_t(25));
                CHECK(act.setting() == ActuatorAnalog::value_t(25));

                cAct.setting(10);
                CHECK(cAct.setting() == ActuatorAnalog::value_t(15));
                CHECK(act.setting() == ActuatorAnalog::value_t(15));

                AND_WHEN("All constraints are removed, the actuator is not clipped anymore")
                {
                    cAct.removeAllConstraints();

                    cAct.setting(50);
                    CHECK(cAct.setting() == ActuatorAnalog::value_t(50));
                    CHECK(act.setting() == ActuatorAnalog::value_t(50));

                    cAct.setting(10);
                    CHECK(cAct.setting() == ActuatorAnalog::value_t(10));
                    CHECK(act.setting() == ActuatorAnalog::value_t(10));
                }
            }
        }
    }
}
