#include "MDNS.h"

bool MDNS::setHostname(String hostname) {
  bool success = true;
  String status = "Ok";

  if (labels[HOSTNAME]) {
    status = "Hostname already set";
    success = false;
  }

  if (success && hostname.length() < MAX_LABEL_SIZE && isAlphaDigitHyphen(hostname)) {
    aRecord = new ARecord();

    HostNSECRecord * hostNSECRecord = new HostNSECRecord();

    records.push_back(aRecord);
    records.push_back(hostNSECRecord);

    Label * label = new HostLabel(aRecord, hostNSECRecord, hostname, LOCAL);

    labels[HOSTNAME] = label;

    aRecord->setLabel(label);
    hostNSECRecord->setLabel(label);
  } else {
    status = success? "Invalid hostname" : status;
    success = false;
  }

  return success;
}

bool MDNS::addService(String protocol, String service, uint16_t port, String instance, std::vector<String> subServices) {
  bool success = true;
  String status = "Ok";

  if (!labels[HOSTNAME]) {
    status = "Hostname not set";
    success = false;
  }

  if (success && protocol.length() < MAX_LABEL_SIZE - 1 && service.length() < MAX_LABEL_SIZE - 1 &&
  instance.length() < MAX_LABEL_SIZE && isAlphaDigitHyphen(protocol) && isAlphaDigitHyphen(service) && isNetUnicode(instance)) {

    PTRRecord * ptrRecord = new PTRRecord();
    SRVRecord * srvRecord = new SRVRecord();
    txtRecord = new TXTRecord();
    InstanceNSECRecord * instanceNSECRecord = new InstanceNSECRecord();

    records.push_back(ptrRecord);
    records.push_back(srvRecord);
    records.push_back(txtRecord);
    records.push_back(instanceNSECRecord);

    String serviceString = "_" + service + "._" + protocol;

    Label * protocolLabel = new Label("_" + protocol, LOCAL);

    if (labels[serviceString] == NULL) {
      labels[serviceString] = new ServiceLabel(aRecord, "_" + service, protocolLabel);
    }

    ((ServiceLabel *) labels[serviceString])->addInstance(ptrRecord, srvRecord, txtRecord);

    String instanceString = instance + "._" + service + "._" + protocol;

    labels[instanceString] = new InstanceLabel(srvRecord, txtRecord, instanceNSECRecord, aRecord, instance, labels[serviceString], true);

    for (std::vector<String>::const_iterator i = subServices.begin(); i != subServices.end(); ++i) {
      String subServiceString = "_" + *i + "._sub." + serviceString;

      if (labels[subServiceString] == NULL) {
        labels[subServiceString] = new ServiceLabel(aRecord, "_" + *i, new Label("_sub", labels[serviceString]));
      }

      PTRRecord * subPTRRecord = new PTRRecord();

      subPTRRecord->setLabel(labels[subServiceString]);
      subPTRRecord->setInstanceLabel(labels[instanceString]);

      records.push_back(subPTRRecord);

      ((ServiceLabel *) labels[subServiceString])->addInstance(subPTRRecord, srvRecord, txtRecord);
    }

    ptrRecord->setLabel(labels[serviceString]);
    ptrRecord->setInstanceLabel(labels[instanceString]);
    srvRecord->setLabel(labels[instanceString]);
    srvRecord->setPort(port);
    srvRecord->setHostLabel(labels[HOSTNAME]);
    txtRecord->setLabel(labels[instanceString]);
    instanceNSECRecord->setLabel(labels[instanceString]);
  } else {
    status = success? "Invalid name" : status;
    success = false;
  }

  return success;
}

void MDNS::addTXTEntry(String key, String value) {
  txtRecord->addEntry(key, value);
}

bool MDNS::begin() {
  // Wait for WiFi to connect
  while (!WiFi.ready()) {
  }

  udp->begin(MDNS_PORT);
  udp->joinMulticast(IPAddress(224, 0, 0, 251));

  // TODO: Probing + announcing

  return true;
}

bool MDNS::processQueries() {
  uint16_t n = udp->parsePacket();

  if (n > 0) {
    buffer->read(udp);

    udp->flush();

    getResponses();

    buffer->clear();

    writeResponses();

    if (buffer->available() > 0) {
      udp->beginPacket(IPAddress(224, 0, 0, 251), MDNS_PORT);

      buffer->write(udp);

      udp->endPacket();
    }
  }

  return n > 0;
}

void MDNS::getResponses() {
  QueryHeader header = readHeader(buffer);

  if ((header.flags & 0x8000) == 0 && header.qdcount > 0) {
    uint8_t count = 0;

    while (count++ < header.qdcount && buffer->available() > 0) {
      Label * label = matcher->match(labels, buffer);

      if (buffer->available() >= 4) {
        uint16_t type = buffer->readUInt16();
        uint16_t cls = buffer->readUInt16();

        if (label != NULL) {

          label->matched(type, cls);
        }
      } else {
        status = "Buffer underflow at index " + buffer->getOffset();
      }
    }
  }
}

MDNS::QueryHeader MDNS::readHeader(Buffer * buffer) {
  QueryHeader header;

  if (buffer->available() >= 12) {
    header.id = buffer->readUInt16();
    header.flags = buffer->readUInt16();
    header.qdcount = buffer->readUInt16();
    header.ancount = buffer->readUInt16();
    header.nscount = buffer->readUInt16();
    header.arcount = buffer->readUInt16();
  }

  return header;
}

void MDNS::writeResponses() {

  uint8_t answerCount = 0;
  uint8_t additionalCount = 0;

  for (std::vector<Record *>::const_iterator i = records.begin(); i != records.end(); ++i) {
    if ((*i)->isAnswerRecord()) {
      answerCount++;
    }
    if ((*i)->isAdditionalRecord()) {
      additionalCount++;
    }
  }

  if (answerCount > 0) {
    buffer->writeUInt16(0x0);
    buffer->writeUInt16(0x8400);
    buffer->writeUInt16(0x0);
    buffer->writeUInt16(answerCount);
    buffer->writeUInt16(0x0);
    buffer->writeUInt16(additionalCount);

    for (std::vector<Record *>::const_iterator i = records.begin(); i != records.end(); ++i) {
      if ((*i)->isAnswerRecord()) {
        (*i)->write(buffer);
      }
    }

    for (std::vector<Record *>::const_iterator i = records.begin(); i != records.end(); ++i) {
      if ((*i)->isAdditionalRecord()) {
        (*i)->write(buffer);
      }
    }
  }

  for (std::map<String, Label *>::const_iterator i = labels.begin(); i != labels.end(); ++i) {
    i->second->reset();
  }

  for (std::vector<Record *>::const_iterator i = records.begin(); i != records.end(); ++i) {
    (*i)->reset();
  }
}

bool MDNS::isAlphaDigitHyphen(String string) {
  bool result = true;

  uint8_t idx = 0;

  while (result && idx < string.length()) {
    uint8_t c = string.charAt(idx++);

    result = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-';
  }

  return result;
}

bool MDNS::isNetUnicode(String string) {
  bool result = true;

  uint8_t idx = 0;

  while (result && idx < string.length()) {
    uint8_t c = string.charAt(idx++);

    result = c >= 0x1f && c != 0x7f;
  }

  return result;
}
