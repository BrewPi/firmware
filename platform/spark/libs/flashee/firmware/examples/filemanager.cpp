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
#pragma SPARK_NO_PREPROCESSOR

/**
 * Illustrates using the FAT storage in eeprom.
 *
 * Press c to create file "abc.txt"
 * Press d to delete the file "abc.txt"
 * Press l to list the contents of the root directory.
 * Press p to print the contents of the file
 *
 * Experiment with different combinations. If you try something that doesn't make sense, such
 * as pressing 'p' when the file has been deleted, you'll see an error.
 * Try also resetting the spark to verify the filesystem is maintained between resets.
 *
 */

#include "application.h"
#include "flashee-eeprom/flashee-eeprom.h"

using namespace Flashee;

FATFS fs;
FRESULT result;
const char* name = "abc.txt";
const char* message = "The world's smallest NAS?";

void printError(Print& p, FRESULT result) {
    switch (result) {
        case FR_OK:
            p.print("OK");
            break;

        case FR_NO_FILESYSTEM:
            p.print("Invalid filesystem");
            break;

        case FR_NO_FILE:
            p.print("File not found");
            break;

        case FR_EXIST:
            p.print("File exists");
            break;

    default:
        p.print("unknown error: ");
        p.print(result);
    }
}

void setup()
{
    Serial.begin(9600);
    result = Devices::createFATRegion(0, 4096*256, &fs);
}

void printResult(Print& p, FRESULT fr, const char* success, const char* fail) {
    if (fr==FR_OK)
        p.println(success);
    else {
        p.print(fail);
        p.print(" :");
        printError(p, fr);
        p.println();
    }
}

void printDate(Print& p, uint16_t date) {
    int year = 1980 + (date >> 9);
    int month = (date >> 5) & 0xF;
    int day = date & 0x1F;
    char buf[128];
    sprintf(buf, "%02d/%02d/%04d", month, day, year);
    p.print(buf);
}

void printTime(Print& p, uint16_t time) {
    int hour = time >> 11;
    int min = (time >> 5) & 0x3F;
    int sec  = (time & 0x1F)*2;
    char buf[128];
    sprintf(buf, "%02d:%02d:%02d", hour, min, sec);
    p.print(buf);
}


void loop()
{
    if (!Serial.available())    // typed something?
        return;

    static bool loggedCreateFS = false;
    if (!loggedCreateFS) {
        loggedCreateFS = true;
        if (result!=FR_OK) {
            Serial.print("Error mounting filesystem: ");
            printError(Serial, result);
            Serial.println();
        }
        else {
            Serial.println("FAT filesystem successfully mounted.");
            Serial.println("Commands: ");
            Serial.println("c - create file abc.txt");
            Serial.println("d - delete file abc.txt");
            Serial.println("l - list files and show used/free space");
            Serial.println("p - print file contents");
        }
    }

    if (result!=FR_OK)      // don't do anything if the filesystem wasn't created
        return;


    char c = Serial.read();

    FIL fil;
    DIR dir;
    FILINFO info;
    FRESULT fr;
    UINT dw;
    DWORD size, tot_sect, fre_sect, total;
    FATFS* pfs;
    char buf[128];
    switch (c) {
        case 'd':       // delete file
            Serial.println("cmd: delete file abc.txt");
            fr = f_unlink(name);
            printResult(Serial, fr, "Deleted file abc.txt", "Unable to delete file");
            break;

        case 'c':       // create file
            Serial.println("cmd: create file abc.txt");
            // this syntax might look strange - it allows you to chain multiple commands together, stopping at
            // the first one that returns something other than FR_OK
            ((fr=f_open(&fil, name, FA_WRITE|FA_CREATE_NEW))==FR_OK) &&
            ((fr=f_write(&fil, message, strlen(message)+1, &dw))==FR_OK) &&
            ((fr=f_close(&fil))==FR_OK);
            printResult(Serial, fr, "Created file abc.txt", "Unable to create file");
            break;

        case 'p':   // print file
            Serial.println("cmd: print file abc.txt");
            ((fr=f_open(&fil, name, FA_READ|FA_OPEN_EXISTING))==FR_OK) &&
            ((fr=f_read(&fil, buf, sizeof(buf), &dw))==FR_OK) &&
            ((fr=f_close(&fil))==FR_OK);
            printResult(Serial, fr, buf, "Unable to read file");
            break;

        case 'l':   // list root folder
            Serial.println("cmd: dir");
            fr=f_opendir(&dir, "");
            if (fr!=FR_OK) {
                printResult(Serial, fr, "", "Unable to list root directory");
                break;
            }
            total = 0;
            for (;;) {  // forever (or until break)
                fr = f_readdir(&dir, &info);
                if (fr!=FR_OK || !*info.fname)
                    break;
                total += info.fsize;
                printDate(Serial, info.fdate);
                Serial.print(' ');
                printTime(Serial, info.ftime);
                Serial.print("     ");
                Serial.print(info.fsize);
                Serial.print(' ');
                Serial.println(info.fname);
            }

            fr  = f_getfree("", &size, &pfs);
            if (fr!=FR_OK) {
                printResult(Serial, fr, "", "Unable to get free space");
                break;
            }

            /* Get total sectors and free sectors */
            tot_sect = (pfs->n_fatent - 2) * pfs->csize;
            fre_sect = size * pfs->csize;

            /* Print the free space (assuming 512 bytes/sector) */
            sprintf(buf, "%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);
            Serial.println(buf);
            break;
    }
    Serial.println();

}