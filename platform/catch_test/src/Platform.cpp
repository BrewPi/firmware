#include "Platform.h"

bool platform_init(){
    return false;
}

static uint8_t device_id[12] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB};

/**
 * In the cbox app, this is called as part of global construction, which is
 * too early for the gcc device to have feched the device id, so it's initialized
 * properly in platform_init() after the gcc command args have been parsed.
 */
void platform_device_id(data_block_ref& id)
{
    id.data = device_id;
    id.size = 12;
}
