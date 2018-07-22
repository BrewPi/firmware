#include "Integration.h"
#include "SystemProfile.h"
#include "Comms.h"
#include "Ticks.h"
#include "TicksObject.h"

namespace cbox {

#if CONTROLBOX_STATIC

#if 0
static SystemProfile SystemProfile;

/**
 * Initialize the controlbox.
 */
void controlbox_setup(size_t loadProfileDelay)
{
    systemProfile.initialize();
	Comms::init();

	if (loadProfileDelay)
	{
		ticks_millis_t start = ticks.millis();
		while ((ticks.millis()-start)<loadProfileDelay) {
			Comms::receive();
		}
	}
	systemProfile.applyActiveProfilesFromEeprom();
}

/**
 * prepare: start of a new control loop and determine how long any asynchronous operations will take.
 * update: fetch data from the environment, read sensor values, compute settings etc..
 */
void process()
{
	container_id ids[MAX_CONTAINER_DEPTH];

    prepare_t d = 0;
    Container* root = SystemProfile::rootContainer();
    if (root)
        d = root->prepare();

    uint32_t end = ticks.millis()+d;
    while (ticks.millis()<end) {
        Comms::receive();
    }

    Container* root2 = SystemProfile::rootContainer();
        // root may have been changed by commands, so original prepare may not be valid
        // should watch out for newly created objects, since these will then also need preparing
	if (root==root2 && root) {
        root->update();

        if (logValuesFlag)
        {
            logValuesFlag = false;
            logValues(ids);
        }
    }
}

/*
 * Manages the control cycle for components:
 *
 */
void controlbox_loop(void)
{
	process();
	Comms::receive();
}

#endif
#endif
} // end namespace cbox
