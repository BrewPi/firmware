#include "application.h"

#ifndef _INCL_LABEL
#define _INCL_LABEL

#include "Buffer.h"
#include "Record.h"
#include <map>
#include <vector>

#define DOT '.'

#define END_OF_NAME 0x0
#define LABEL_POINTER 0xc0
#define MAX_LABEL_SIZE 63
#define INVALID_OFFSET -1

#define UNKNOWN_NAME -1
#define BUFFER_UNDERFLOW -2

class Label {
private:

  class Iterator;

public:
  class Matcher {
  public:
    Label * match(std::map<String, Label *> labels, Buffer * buffer);
  };

  Label(String name, Label * nextLabel = NULL, bool caseSensitive = false);

  uint8_t getSize();

  uint8_t getWriteSize();

  void write(Buffer * buffer);

  virtual void matched(uint16_t type, uint16_t cls);

  void reset();

private:
  class Reader {
  public:
    Reader(Buffer * buffer);

    bool hasNext();

    uint8_t next();

    bool endOfName();
  private:
    Buffer * buffer;
    uint8_t c = 1;
  };

  class Iterator {
  public:
    Iterator(Label * label);

    bool match(uint8_t c);

    bool matched();

    Label * getStartLabel();

  private:
    Label * startLabel;
    Label * label;
    uint8_t size;
    uint8_t offset = 0;
    bool matches = true;

    bool equalsIgnoreCase(uint8_t c);
  };

  uint8_t * EMPTY_DATA = { END_OF_NAME };
  uint8_t * data;
  bool caseSensitive;
  Label * nextLabel;
  int16_t writeOffset = INVALID_OFFSET;
};

class HostLabel : public Label {

public:

  HostLabel(Record * aRecord, Record * nsecRecord, String name, Label * nextLabel = NULL, bool caseSensitive = false);

  virtual void matched(uint16_t type, uint16_t cls);

private:
  Record * aRecord;
  Record * nsecRecord;
};

class ServiceLabel : public Label {

public:

  ServiceLabel(Record * aRecord, String name, Label * nextLabel = NULL, bool caseSensitive = false);

  void addInstance(Record * ptrRecord, Record * srvRecord, Record * txtRecord);

  virtual void matched(uint16_t type, uint16_t cls);

private:
  Record * aRecord;
  std::vector<Record *> ptrRecords;
  std::vector<Record *> srvRecords;
  std::vector<Record *> txtRecords;
};

class InstanceLabel : public Label {

public:

  InstanceLabel(Record * srvRecord, Record * txtRecord, Record * nsecRecord, Record * aRecord, String name, Label * nextLabel = NULL, bool caseSensitive = false);

  virtual void matched(uint16_t type, uint16_t cls);

private:
  Record * srvRecord;
  Record * txtRecord;
  Record * nsecRecord;
  Record * aRecord;
};

#endif
