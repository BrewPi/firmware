/**
 ******************************************************************************
  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */
#include "application.h"

retained int variable = 10;

void setup()
{
	Serial.begin(9600);
	if (int(&variable) < 0x30000000) {
		Serial.printlnf("ERROR: expected variable in backup memory, but was at %x", &variable);
		return;
	}

	if (!System.featureEnabled(FEATURE_WARM_START))
	{
		// cold start
		Serial.println("Cold start");
		if (variable!=10)
		{
			Serial.printlnf("ERROR: expected retained variable to be initialized to 10, but was %d", variable);
		}
		else
		{
			Serial.println("Retained variable initialized to 10. Restarting");
			System.enableFeature(FEATURE_RETAINED_MEMORY);
			variable++;
			System.reset();
		}
	}
	else {
		Serial.println("Warm start");
		if (variable!=11)
		{
			Serial.printlnf("ERROR: expected retained variable to be 11, but was %d", variable);
		}
		else
		{
			Serial.println("SUCCESS: Retained variable retained after reset");
		}
	}
}
