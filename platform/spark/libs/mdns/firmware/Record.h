#include "application.h"

#ifndef _INCL_RECORD
#define _INCL_RECORD

#include "Buffer.h"
#include <vector>

#define IN_CLASS 1

#define A_TYPE 0x01
#define PTR_TYPE 0x0c
#define TXT_TYPE 0x10
#define AAAA_TYPE 0x1c
#define SRV_TYPE 0x21
#define NSEC_TYPE 0x2f

#define ANY_TYPE 0xFF

#define TTL_2MIN 120
#define TTL_75MIN 4500

#define IP_SIZE 4

class Label;

class Record {

public:

  void setLabel(Label * label);

  void setAnswerRecord();

  bool isAnswerRecord();

  void setAdditionalRecord();

  bool isAdditionalRecord();

  void setKnownRecord();

  void write(Buffer * buffer);

  void reset();

protected:

  Record(uint16_t type, uint32_t ttl);

  Label * getLabel();

  virtual void writeSpecific(Buffer * buffer) = 0;

private:

  Label * label;
  uint16_t type;
  uint32_t ttl;
  bool answerRecord = false;
  bool additionalRecord = false;
  bool knownRecord = false;
};

class ARecord : public Record {

public:

  ARecord();

  virtual void writeSpecific(Buffer * buffer);
};

class NSECRecord : public Record {

public:

  NSECRecord();

  virtual void writeSpecific(Buffer * buffer) = 0;
};

class HostNSECRecord : public NSECRecord {

public:

  HostNSECRecord();

  virtual void writeSpecific(Buffer * buffer);
};

class InstanceNSECRecord : public NSECRecord {

public:

  InstanceNSECRecord();

  virtual void writeSpecific(Buffer * buffer);
};

class PTRRecord : public Record {

public:

  PTRRecord();

  virtual void writeSpecific(Buffer * buffer);

  void setInstanceLabel(Label * label);

private:

  Label * instanceLabel;

};

class SRVRecord : public Record {

public:

  SRVRecord();

  virtual void writeSpecific(Buffer * buffer);

  void setHostLabel(Label * label);

  void setPort(uint16_t port);

private:

  Label * hostLabel;
  uint16_t port;
};

class TXTRecord : public Record {

public:

  TXTRecord();

  virtual void writeSpecific(Buffer * buffer);

  void addEntry(String key, String value = NULL);

private:

  std::vector<String> data;
};

#endif
