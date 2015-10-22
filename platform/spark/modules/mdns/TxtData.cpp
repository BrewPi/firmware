#include "TxtData.h"

bool TxtData::addEntry(String key, String value) {
    uint8_t * oldData = data;
    uint16_t oldSize = size;

    uint8_t addedSize = key.length() + value.length() + 2;

    size += addedSize;

    data = (uint8_t *) malloc(size);

    if (data) {
        if (oldData) {
            for (int i = 0; i < oldSize; i++) {
                data[i] = oldData[i];
            }
        }
        int idx = oldSize;

        data[idx++] = addedSize - 1;

        for (uint8_t i = 0; i < key.length(); i++) {
            data[idx++] = key.charAt(i);
        }

        data[idx++] = '=';

        for (uint8_t i = 0; i < value.length(); i++) {
            data[idx++] = value.charAt(i);
        }
    }

    if (oldData) {
        free(oldData);
    }

    return data != NULL;
}

void TxtData::write(Buffer * buffer) {
    buffer->writeUInt16(size);
    for (uint16_t i = 0; i < size; i++) {
        buffer->writeUInt8(data[i]);
    }
}
