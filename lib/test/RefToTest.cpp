/*
* Copyright 2015 BrewPi/Elco Jacobs.
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

#include <boost/test/unit_test.hpp>

#include "runner.h"
#include <string>

#include "ActuatorInterfaces.h"
#include "ActuatorMocks.h"
#include "TempSensorMock.h"
#include "SensorSetPointPair.h"
#include "RefTo.h"
#include "SetPoint.h"
#include "ActuatorOffset.h"

struct RefToFixture{
public:
    RefToFixture(){
        act1 = new ActuatorBool();
        act2 = new ActuatorBool();
        temp = new TempSensorMock(20.0);

        devices.push_back(act1);
        devices.push_back(act2);
        devices.push_back(temp);
    }
    ~RefToFixture(){
        delete act1;
        delete act2;
        delete temp;
    }

    std::vector<Interface*> devices;
    ActuatorBool * act1;
    ActuatorBool * act2;
    TempSensorMock * temp;
};

// Define a simple lookup object.
// This lookup object simply stores a pointer to a vector and an index.
// Applications can define more complex lookup objects that search object containers
// The only requirement is that operator () returns an Interface pointer
struct VectorIndexLookup {
    VectorIndexLookup(unsigned int i, std::vector<Interface*>& vect) : i(i), vect(vect){};
    ~VectorIndexLookup()= default;

    Interface * operator()(){
        if(i < vect.size()){
            return vect[i];
        }
        return nullptr;
    }

private:
    unsigned int i;
    std::vector<Interface*>& vect;
};

// next line sets up the fixture for each test case in this suite
BOOST_FIXTURE_TEST_SUITE( RefToTest, RefToFixture )

BOOST_AUTO_TEST_CASE(unset_RefTo_returns_default_actuator) {
    RefTo<ActuatorDigital> ref;

    BOOST_CHECK_EQUAL(ref.get(), defaultTarget<ActuatorDigital>());
}

BOOST_AUTO_TEST_CASE(RefTo_pointing_to_wrong_type_returns_default_actuator_for_requested_interface) {
    auto lookup = VectorIndexLookup(1, devices);
    RefTo<ActuatorDigital> ref(lookup);

    BOOST_CHECK_EQUAL(ref.get(), act2); // device at index 1 is act2

    RefTo<TempSensor> refWrongType(lookup);

    BOOST_CHECK_EQUAL(refWrongType.get(), defaultTarget<TempSensor>()); // device at index 1 is act2
}

BOOST_AUTO_TEST_CASE(RefTo_can_be_used_as_a_ActuatorDigital) {
    auto lookup = VectorIndexLookup(1, devices);
    RefTo<ActuatorDigital> ref(lookup);

    BOOST_CHECK(act2->getState() == ActuatorDigital::State::Inactive); // default value is false
    ref().setState(ActuatorDigital::State::Active);
    BOOST_CHECK(act2->getState() == ActuatorDigital::State::Active); // value has been set to true via reference
}

BOOST_AUTO_TEST_CASE(RefTo_can_set_to_a_different_lookup) {
    auto lookup = VectorIndexLookup(1, devices);
    RefTo<ActuatorDigital> ref(lookup);
    BOOST_CHECK_EQUAL(ref.get(), act2); // device at index 1 is act2

    auto lookup2 = VectorIndexLookup(0, devices);
    ref.setLookup(lookup2);

    BOOST_CHECK_EQUAL(ref.get(), act1); // device at index 1 is now act1
}

BOOST_AUTO_TEST_CASE(Lookup_can_be_deleted_after_passing_to_reference) {
    auto lookup = new VectorIndexLookup(1, devices);
    RefTo<ActuatorDigital> ref(*lookup);

    delete lookup;

    BOOST_CHECK_EQUAL(ref.get(), act2); // device at index 1 is act2
}

BOOST_AUTO_TEST_CASE(RefTo_SetPointActuator) {
    auto sp1 = SetPointSimple(20.0);
    auto sp2 = SetPointSimple(20.0);
    auto sensor1 = TempSensorMock(20.0);
    auto sensor2 = TempSensorMock(20.0);

    auto pair1 = SensorSetPointPair(sensor1, sp1);
    auto pair2 = SensorSetPointPair(sensor2, sp2);
    Interface * spa = new ActuatorOffset(pair1, pair2);
    auto lookup = PtrLookup(spa);
    RefTo<ActuatorAnalog> ref(lookup);

    BOOST_CHECK_EQUAL(ref.get(), spa);
}



BOOST_AUTO_TEST_SUITE_END()

