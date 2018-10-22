#include "application.h"

#ifndef _INCL_BUFFER
#define _INCL_BUFFER

#define INVALID_MARK_OFFSET 0xffff

class Buffer {
public:
  Buffer(uint16_t size);

  uint16_t available();
  
  void mark();
  void reset();
  void setOffset(uint16_t offset);
  uint16_t getOffset();

  void read(UDP * udp);

  uint8_t readUInt8();
  uint16_t readUInt16();

  void write(UDP * udp);

  void writeUInt8(uint8_t value);
  void writeUInt16(uint16_t value);
  void writeUInt32(uint32_t value);

  void clear();

private:

  uint8_t * data;
  uint16_t size;

  uint16_t limit = 0;
  uint16_t offset = 0;
  uint16_t markOffset = INVALID_MARK_OFFSET;
};

#endif
