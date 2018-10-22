#include "Buffer.h"

Buffer::Buffer(uint16_t size) {
  this->data = (uint8_t *) malloc(size);
  this->size = data != NULL? size : 0;
}

uint16_t Buffer::available() {
  return offset < limit? limit - offset : offset - limit;
}

void Buffer::mark() {
  if (markOffset == INVALID_MARK_OFFSET) {
    markOffset = offset;
  }
}

void Buffer::reset() {
  if (markOffset != INVALID_MARK_OFFSET) {
    offset = markOffset;
    markOffset = INVALID_MARK_OFFSET;
  }
}

void Buffer::setOffset(uint16_t offset) {
  this->offset = offset;
}

uint16_t Buffer::getOffset() {
  return offset;
}

void Buffer::read(UDP * udp) {
  offset = 0;
  limit = udp->read(data, size);
}

uint8_t Buffer::readUInt8() {
  return data[offset++];
}

uint16_t Buffer::readUInt16() {
  return readUInt8() << 8 | readUInt8();
}

void Buffer::writeUInt8(uint8_t value) {
  if (offset < size) {
    data[offset++] = value;
  }
}

void Buffer::writeUInt16(uint16_t value) {
  writeUInt8(value >> 8);
  writeUInt8(value);
}

void Buffer::writeUInt32(uint32_t value) {
  writeUInt8(value >> 24);
  writeUInt8(value >> 16);
  writeUInt8(value >> 8);
  writeUInt8(value);
}

void Buffer::write(UDP * udp) {
  udp->write(data, offset);

  offset = 0;
}

void Buffer::clear() {
  offset = 0;
  limit = 0;
}
