#include "Record.h"

#include "Label.h"

Record::Record(uint16_t type, uint32_t ttl) {
  this->type = type;
  this->ttl = ttl;
}

void Record::setLabel(Label * label) {
  this->label = label;
}

void Record::setAnswerRecord() {
  this->answerRecord = true;
}

bool Record::isAnswerRecord() {
  return answerRecord && !knownRecord;
}

void Record::setAdditionalRecord() {
  this->additionalRecord = true;
}

bool Record::isAdditionalRecord() {
  return additionalRecord && !answerRecord && !knownRecord;
}

void Record::setKnownRecord() {
  this->knownRecord = true;
}

void Record::write(Buffer * buffer) {
  label->write(buffer);
  buffer->writeUInt16(type);
  buffer->writeUInt16(IN_CLASS);
  buffer->writeUInt32(ttl);
  writeSpecific(buffer);
}

void Record::reset() {
  this->answerRecord = false;
  this->additionalRecord = false;
  this->knownRecord = false;
}

Label * Record::getLabel() {
  return label;
}

ARecord::ARecord():Record(A_TYPE, TTL_2MIN) {
}

void ARecord::writeSpecific(Buffer * buffer) {
  buffer->writeUInt16(4);
  IPAddress ip = WiFi.localIP();
  for (int i = 0; i < IP_SIZE; i++) {
    buffer->writeUInt8(ip[i]);
  }
}

NSECRecord::NSECRecord():Record(NSEC_TYPE, TTL_2MIN) {
}

HostNSECRecord::HostNSECRecord():NSECRecord() {
}

void HostNSECRecord::writeSpecific(Buffer * buffer) {
  buffer->writeUInt16(5);
  getLabel()->write(buffer);
  buffer->writeUInt8(0);
  buffer->writeUInt8(1);
  buffer->writeUInt8(0x40);
}

InstanceNSECRecord::InstanceNSECRecord():NSECRecord() {
}

void InstanceNSECRecord::writeSpecific(Buffer * buffer) {
  buffer->writeUInt16(9);
  getLabel()->write(buffer);
  buffer->writeUInt8(0);
  buffer->writeUInt8(5);
  buffer->writeUInt8(0);
  buffer->writeUInt8(0);
  buffer->writeUInt8(0x80);
  buffer->writeUInt8(0);
  buffer->writeUInt8(0x40);
}

PTRRecord::PTRRecord():Record(PTR_TYPE, TTL_75MIN) {
}

void PTRRecord::writeSpecific(Buffer * buffer) {
  buffer->writeUInt16(instanceLabel->getWriteSize());
  instanceLabel->write(buffer);
}

void PTRRecord::setInstanceLabel(Label * label) {
  instanceLabel = label;
}

SRVRecord::SRVRecord():Record(SRV_TYPE, TTL_2MIN) {
}

void SRVRecord::writeSpecific(Buffer * buffer) {
  buffer->writeUInt16(6 + hostLabel->getWriteSize());
  buffer->writeUInt16(0);
  buffer->writeUInt16(0);
  buffer->writeUInt16(port);
  hostLabel->write(buffer);
}

void SRVRecord::setHostLabel(Label * label) {
  hostLabel = label;
}

void SRVRecord::setPort(uint16_t port) {
  this->port = port;
}

TXTRecord::TXTRecord():Record(TXT_TYPE, TTL_75MIN) {
}

void TXTRecord::addEntry(String key, String value) {
  String entry = key;

  if (value != NULL) {
    entry += '=';
    entry += value;
  }

  data.push_back(entry);
}

void TXTRecord::writeSpecific(Buffer * buffer) {
  uint16_t size = 0;

  std::vector<String>::const_iterator i;

  for(i = data.begin(); i != data.end(); ++i) {
    size += i->length() + 1;
  }

  buffer->writeUInt16(size);

  for(i = data.begin(); i != data.end(); ++i) {
    uint8_t length = i->length();

    buffer->writeUInt8(length);

    for (uint8_t idx = 0; idx < length; idx++) {
      buffer->writeUInt8(i->charAt(idx));
    }
  }
}
