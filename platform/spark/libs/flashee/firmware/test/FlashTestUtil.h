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

#ifndef FLASHTESTUTIL_H
#define	FLASHTESTUTIL_H

#include "flashee-eeprom.h"

namespace Flashee {

class FlashTestUtil {
    
public:    
    static ::testing::AssertionResult assertSamePagewise(FlashDevice& expected, FlashDevice& actual) {
        flash_addr_t offset = 0;
        flash_addr_t length = expected.length();
        page_size_t pageSize = expected.pageSize();
        uint8_t actualBuf[512];
        uint8_t expectedBuf[512];
        while (offset<length) {
            for (int p=0; p<pageSize;) {
                page_size_t toRead = min(pageSize-p, sizeof(actualBuf));
                memset(actualBuf, 0, sizeof(actualBuf));
                memset(expectedBuf, 0, sizeof(expectedBuf));
                if(!actual.read(actualBuf, offset, toRead)) 
                    return ::testing::AssertionFailure() << "unable to read actual data at " << offset+p << " length " << toRead;
                if (!expected.read(expectedBuf, offset, toRead))
                    return ::testing::AssertionFailure() << "unable to read expected data at " << offset+p << " length " << toRead;
                for (int i=0; i<toRead; i++) {
                    if (actualBuf[i]!=expectedBuf[i]) {
                        ::testing::AssertionResult result = 
                            ::testing::AssertionFailure() << int(actualBuf[i]) << "!=" << int(expectedBuf[i]) << " at offset " << offset+p+i << "(page "<<offset/pageSize<< " offset "<<(p+i)<< ")";
                        actual.read(expectedBuf, offset, toRead);   // debugging - allow to rerun read code
                        return result;
                    }
                }                
                p+=toRead;
            }
            offset += pageSize;
        }
        return ::testing::AssertionSuccess();
    }

};

}

#endif	/* FLASHTESTUTIL_H */

