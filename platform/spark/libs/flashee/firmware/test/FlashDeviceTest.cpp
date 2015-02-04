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
#include "FlashDeviceTest.h"

template <>
FlashDevice* CreateFlashDevice<FakeFlashDevice>() {
    return new FakeFlashDevice(384, 4096);
}

template <>
FlashDevice* CreateFlashDevice<LogicalPageMapper<> >() {
    FakeFlashDevice* storage = new FakeFlashDevice(256, 4096);  // has to have at least one page more
    storage->eraseAll();
    LogicalPageMapper<>* mapper = new LogicalPageMapper<>(*storage, storage->pageCount()-2);
    return mapper;
}

template <>
FlashDevice* CreateFlashDevice<MultiWriteFlashStore>() {
    FakeFlashDevice* storage = new FakeFlashDevice(256, 4096);  // has to have at least two pages more
    LogicalPageMapper<>* mapper = new LogicalPageMapper<>(*storage, storage->pageCount()-2);
    MultiWriteFlashStore* eeprom = new MultiWriteFlashStore(*mapper);
    return eeprom;
}

template <>
FlashDevice* CreateFlashDevice<SinglePageWear>() {
    FakeFlashDevice* storage = new FakeFlashDevice(256, 4096);  // has to have at least two pages more    
    SinglePageWear* eeprom = new SinglePageWear(*storage);
    return eeprom;
}

INSTANTIATE_TYPED_TEST_CASE_P(Fake, FlashDeviceTest, FakeFlashDevice);
INSTANTIATE_TYPED_TEST_CASE_P(FakeLogicalMapper, FlashDeviceTest, LogicalPageMapper<>);
INSTANTIATE_TYPED_TEST_CASE_P(FakeEepromEmulation, FlashDeviceTest, MultiWriteFlashStore);
INSTANTIATE_TYPED_TEST_CASE_P(FakeSinglePageWear, FlashDeviceTest, SinglePageWear);

