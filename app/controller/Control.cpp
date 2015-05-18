/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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



#include "Control.h"

#include "TemperatureFormats.h"

Control::Control(){}

void Control::initBackwardsCompatible(void)
{
    Pid * fridgePid = new Pid;
    Pid * beerPid   = new Pid;

    beerPid -> setConstants(doubleToTempDiff(5.0), doubleToTempDiff(0.2), doubleToTempDiff(-1.5));
    fridgePid -> setConstants(doubleToTempDiff(5.0), doubleToTempDiff(0.2), doubleToTempDiff(-1.5));

    beerPid->setInputFilter(4u);
    beerPid->setSlopeFilter(4u);
    fridgePid->setInputFilter(4u);
    fridgePid->setSlopeFilter(4u);

    beerPid->setMinMax(intToTemp(-10),intToTemp(10));
    pids.push_back(beerPid);
    pids.push_back(fridgePid);
}
