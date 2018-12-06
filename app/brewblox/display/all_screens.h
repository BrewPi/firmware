#pragma once

#include "d4d.hpp"

// forward declare all screens so the linker doesn't loose them for some strange reason.
// include in file defining screen and in file using screen
extern const D4D_SCREEN screen_startup;