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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "flashee-eeprom.h"
#include "FlashTestUtil.h"

using namespace Flashee;

class FatTest : public ::testing::Test {

};

void assertFileExists(const TCHAR* name, const char* c) {
    FIL fp; UINT dw;
    SCOPED_TRACE(testing::Message() << "file " << name);
    ASSERT_EQ(FR_OK, f_open(&fp, name, FA_OPEN_EXISTING|FA_READ)) << "expected file to exist";
    char buf[50];
    ASSERT_EQ(FR_OK, f_read(&fp, buf, 50, &dw)) << "could not read from file";
    ASSERT_STREQ(c, buf);
    ASSERT_EQ(FR_OK, f_close(&fp));
}

void assertFileNotExists(const TCHAR* name, const char* c) {
    FIL fp;
    ASSERT_EQ(FR_NO_FILE, f_open(&fp, name, FA_OPEN_EXISTING|FA_READ)) << "expecting file to exist";
}

void assertCreateFile(const TCHAR* name, const char* c) {
    FIL fp; UINT dw;
    ASSERT_EQ(FR_OK, f_open(&fp, name, FA_CREATE_NEW|FA_WRITE));            
    ASSERT_EQ(FR_OK, f_write(&fp,c, strlen(c)+1, &dw));        
    ASSERT_EQ(FR_OK, f_close(&fp));

    assertFileExists(name, c);
}

TEST(CreateFSTest, FilesystemIsPersisted) {    

    FakeFlashDevice fake(20, 4096);
    fake.eraseAll();
    LogicalPageMapper<> mapper(fake, fake.pageCount()-2);
    PageSpanFlashDevice* span = new PageSpanFlashDevice(mapper);
    FATFS fs;
    f_setFlashDevice(span, &fs, FORMAT_CMD_FORMAT_IF_NEEDED);
    printf("Created filesystem\r\n");
    assertCreateFile("abcd.txt", "hello world!");    
    printf("Created file\r\n");
    assertFileExists("abcd.txt", "hello world!");    
    printf("asserted File exists\r\n");
    LogicalPageMapper<> mapper2(fake, fake.pageCount()-2);
    PageSpanFlashDevice* span2 = new PageSpanFlashDevice(mapper2);

    char buf[20];
    char buf2[20];
    mapper.read(buf, 0, 20);    

    mapper2.read(buf2, 0, 20);
    ASSERT_TRUE(!memcmp(buf, buf2, 20)) << " mapper 2 different";

    //ASSERT_TRUE(FlashTestUtil::assertSamePagewise(mapper, mapper2));

    mapper2.read(buf2, 0, 20);
    ASSERT_TRUE(!memcmp(buf, buf2, 20)) << " mapper 2 different after compare";
    
    FATFS fs2;
    f_setFlashDevice(span2, &fs2, FORMAT_CMD_NONE);

    mapper2.read(buf2, 0, 20);
    ASSERT_TRUE(!memcmp(buf, buf2, 20)) << " mapper 2 different after create fs";

    printf("Created filesystem2\r\n");    

    assertFileExists("abcd.txt", "hello world!");    
    printf("Created checkFileExists\r\n");

}

TEST(CreateFSTest, FilesystemIsPersisted2) {    

    FakeFlashDevice fake(256, 4096);
    fake.eraseAll();
    LogicalPageMapper<> mapper(fake, 254);
    PageSpanFlashDevice* span = new PageSpanFlashDevice(mapper);

    // now create another filesystem using just the fake flash
    FakeFlashDevice* fake2 = new FakeFlashDevice(span->pageCount(), span->pageSize(), true);
    fake2->eraseAll();
    FATFS fs2;
    f_setFlashDevice(fake2, &fs2, FORMAT_CMD_FORMAT_IF_NEEDED);    
    assertCreateFile("abcd.txt", "hello world!");    

    FATFS fs;
    f_setFlashDevice(span, &fs, FORMAT_CMD_FORMAT_IF_NEEDED);
    assertCreateFile("abcd.txt", "hello world!");    

    ASSERT_TRUE(FlashTestUtil::assertSamePagewise(*span, *fake2));
}



TEST(CreateFSTest, DeviceNotFormattedReturnsErrorCode) {
    ASSERT_TRUE(Devices::userFlash().eraseAll());
    FATFS fs;
    FRESULT result = Devices::createFATRegion(0*4096, 128*4096, &fs, FORMAT_CMD_NONE);
    ASSERT_EQ(FR_NO_FILESYSTEM, result) << "Expected no filesystem on uninitialized flash";
}

TEST(CreateFSTest, DeviceNotFormattedIsFormattedByDefault) {
    ASSERT_TRUE(Devices::userFlash().eraseAll());
    FATFS fs;
    FRESULT result = Devices::createFATRegion(0*4096, 128*4096, &fs);
    ASSERT_EQ(FR_OK, result) << "Expected no filesystem on uninitialized flash";
    
    DIR d; FILINFO i;    
    ASSERT_EQ(FR_OK, f_opendir(&d, ""));
    ASSERT_EQ(FR_OK, f_readdir(&d, &i));
    ASSERT_STREQ("", i.fname);           // no files    
}

TEST(CreateFSTest, DeviceNotFormattedIfAlreadyFormatted) {
    FATFS fs;
    ASSERT_TRUE(Devices::userFlash().eraseAll());
    ASSERT_EQ(FR_OK, Devices::createFATRegion(0*4096, 128*4096, &fs));
    
    FIL fil;    // create a file
    const char* filename = "abc.txt";
    ASSERT_EQ(FR_OK, f_open(&fil, filename, FA_CREATE_NEW));
    ASSERT_EQ(FR_OK, f_close(&fil));
    ASSERT_EQ(FR_OK, f_stat(filename, NULL)) << "expected file to exist after create";
    
    FATFS fs2;    
    ASSERT_EQ(FR_OK, Devices::createFATRegion(0*4096, 128*4096, &fs2));    
    ASSERT_EQ(FR_OK, f_stat(filename, NULL)) << "expected file to exist after recreating flash region";    
}

TEST(CreateFSTest, DeviceClearedWhenFormatRequested) {
    Devices::userFlash().eraseAll();
    FATFS fs;
    ASSERT_EQ(FR_OK, Devices::createFATRegion(0*4096, 128*4096, &fs));
    
    FIL fil;    // create a file
    const char* filename = "abc.txt";
    ASSERT_EQ(FR_OK, f_open(&fil, filename, FA_CREATE_NEW));
    ASSERT_EQ(FR_OK, f_close(&fil));
    ASSERT_EQ(FR_OK, f_stat(filename, NULL)) << "expected file to exist after create";
    
    FATFS fs2;
    ASSERT_EQ(FR_OK, Devices::createFATRegion(0*4096, 128*4096, &fs2, FORMAT_CMD_FORMAT));    
    ASSERT_EQ(FR_NO_FILE, f_stat(filename, NULL)) << "expected file to not exist after recreating and formatting the flash region";    
}

class FSTest : public ::testing::Test {
    FATFS fs; FIL fp; UINT dw;

public:
    FSTest() {        
    }
    
    void SetUp() {
        // format 
        ASSERT_TRUE(Devices::userFlash().eraseAll());
        createFS();
    }    

    void createFS() {
        memset(&fs, 0, sizeof(fs));
        ASSERT_EQ(FR_OK, Devices::createFATRegion(0*4096, 128*4096, &fs));
    }

};

TEST_F(FSTest, CanCreateFile) {
    assertCreateFile("abcd.txt", "hello world!");
}

TEST_F(FSTest, CreatedFileIsPersisted) {
    assertCreateFile("abcd.txt", "hello world!");    
    assertCreateFile("abc.txt", "foo bar");    

    assertFileExists("abcd.txt", "hello world!");
    assertFileExists("abc.txt", "foo bar");    

    createFS();
    assertFileExists("abcd.txt", "hello world!");
    assertFileExists("abc.txt", "foo bar");    
}

TEST_F(FSTest, CreatedSingleFileIsPersisted) {
    assertCreateFile("abcd.txt", "hello world!");    
    createFS();
    assertFileExists("abcd.txt", "hello world!");
}

