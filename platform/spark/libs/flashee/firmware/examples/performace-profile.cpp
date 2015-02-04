
#pragma SPARK_NO_PREPROCESSOR
/**
 * Copyright 2014  Matthew McGowan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /**
  * Runs eeprom device tests against the address-erasable eeprom emulation.
  */

#include "flashee-eeprom/flashee-eeprom.h"

using namespace Flashee;

void setup()
{
    Serial.begin(9600);
}

/**
 * Struct for the buffer details. Allows all the required arguments to be passed as a single pointer
 */
struct BufArgs {
    void* buf;
    void* buf2;
    page_size_t bufSize;
    flash_addr_t end;
};

bool eraseAll(FlashDevice* device, void* args) {
    BufArgs& bufArgs = *(BufArgs*)args;
    page_size_t pageSize = device->pageSize();
    bool success = true;
    for (flash_addr_t i=0; i<bufArgs.end; i+=pageSize) {
        success = success && device->erasePage(i);
    }
    return success;
}

bool verify(FlashDevice* device, void* args) {
    BufArgs& bufArgs = *(BufArgs*)args;
    page_size_t bufSize = bufArgs.bufSize;
    bool success = true;
    page_size_t pageSize = device->pageSize();
    for (flash_addr_t p = 0; p<bufArgs.end; ) {
        page_size_t offset = 0;
        page_size_t pageEnd = min(pageSize, bufArgs.end-p);
        while (offset<pageSize) {
            page_size_t toRead = min(pageSize-offset, bufSize);
            success = success && device->readPage(bufArgs.buf2, offset, toRead);
            success = success && !memcmp(bufArgs.buf2, bufArgs.buf, toRead);
            offset += toRead;
        }
        p += pageEnd;
    }
    return success;
}

bool write(FlashDevice* device, void* args) {
    BufArgs& bufArgs = *(BufArgs*)args;
    page_size_t bufSize = bufArgs.bufSize;
    bool success = true;
    page_size_t pageSize = device->pageSize();
    for (flash_addr_t p = 0; p<bufArgs.end; ) {
        page_size_t offset = 0;
        page_size_t pageEnd = min(pageSize, bufArgs.end-p);
        while (offset<pageSize) {
            page_size_t toWrite = min(pageSize-offset, bufSize);
            success = success && device->writePage(bufArgs.buf, offset, toWrite);
            offset += toWrite;
        }
        p += pageEnd;
    }
    return success;
}

bool rewrite(FlashDevice* device, void* args) {
    BufArgs& bufArgs = *(BufArgs*)args;
    page_size_t bufSize = bufArgs.bufSize;
    bool success = true;
    page_size_t pageSize = device->pageSize();
    for (flash_addr_t p = 0; p<bufArgs.end; ) {
        page_size_t offset = 0;
        page_size_t pageEnd = min(pageSize, bufArgs.end-p);
        while (offset<pageSize) {
            page_size_t toWrite = min(pageSize-offset, bufSize);
            success = success && device->writeErasePage(bufArgs.buf, offset, toWrite);
            offset += toWrite;
        }
        p += pageEnd;
    }
    return success;
}

typedef bool (*TimeFunction)(FlashDevice* device, void* args);

void time(TimeFunction fn, FlashDevice* device, void* args, const char* opName, flash_addr_t byteCount) {
    uint32_t start = millis();
    bool success = fn(device, args);
    uint32_t end = millis();
    uint32_t duration = end-start;

    Serial.print(" ");
    Serial.print(opName);
    Serial.print(':');
    if (success) {
        Serial.print(" throughput ");
        Serial.print(byteCount/duration);
        Serial.println(" Kbytes/sec");
    }
    else {
        Serial.println(" N/A (not supported or failed.)");
    }
}


void performaceTestSize(FlashDevice* device, uint8_t* buf, uint8_t* buf2, page_size_t bufSize) {
    flash_addr_t end = device->length();

    Serial.print("Buffer size: ");
    Serial.print(bufSize);
    Serial.print(", total bytes: ");
    Serial.println(end);

    BufArgs args;
    args.buf = buf;
    args.buf2 = buf2;
    args.bufSize = bufSize;
    args.end = end;

    // erase
    time(eraseAll, device, &args, "Erase", end);

    memset(buf, -1, bufSize);
    time(verify, device, & args, "Verify erase", end);

    // write (so non-eeprom devices can be profiled)
    memset(buf, 0xA9, bufSize);
    time(write, device, &args, "Write", end);

    time(verify, device, & args, "Verify write", end);


    // write erase (if supported)
    memset(buf, 0x9A, bufSize);
    time(rewrite, device, &args, "Rewrite", end);

    time(verify, device, & args, "Verify rewrite", end);


    Serial.println();
}

void performanceTestByteRewrite(FlashDevice* device) {
    // erase

}

void performanceTest(FlashDevice* device, const char* name) {
    Serial.print("Performance test: ");
    Serial.println(name);

    uint8_t buf[2048];
    uint8_t buf2[2048];
    performaceTestSize(device, buf, buf2, 128);
    performaceTestSize(device, buf, buf2, 512);
    performaceTestSize(device, buf, buf2, 2048);
    performanceTestByteRewrite(device);

    Serial.println();
}


void loop()
{
    if (Serial.available()) {
        char c = Serial.read();

        if (c=='t') {
            Serial.println("Running tests");
            performanceTest(Devices::createAddressErase(0, 4096*256, 256-32), "Address level erase");
            performanceTest(Devices::createWearLevelErase(0, 4096*256, 256-32), "Wear level page erase");
            performanceTest(Devices::createUserFlashRegion(0, 4096*64), "Basic flash access");
            Serial.println("Test complete.");
        }
    }
}