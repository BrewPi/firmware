#include "MDNS.h"
#include "PiLink.h"

bool MDNS::setHostname(String hostname) {
    bool success = hostname.length() < MAX_LABEL_SIZE && isAlphaDigitHyphen(hostname);

    if (success) {
        labels[HOST_NAME] = new Label(hostname, LOCAL);

        success = labels[HOST_NAME]->getSize() == hostname.length();
    }

    return success;
}

bool MDNS::setService(String protocol, String service, uint16_t port, String instance) {
    bool success = protocol.length() < MAX_LABEL_SIZE - 1 && service.length() < MAX_LABEL_SIZE - 1 &&
        instance.length() < MAX_LABEL_SIZE && isAlphaDigitHyphen(protocol) && isAlphaDigitHyphen(service) && isNetUnicode(instance);

    Label * protoLabel;

    if (success) {
        protoLabel = new Label("_" + protocol, LOCAL);

        success = protoLabel->getSize() == protocol.length() + 1;
    }

    if (success) {
        labels[SERVICE_NAME] = new Label("_" + service, protoLabel);

        success = labels[SERVICE_NAME]->getSize() == service.length() + 1;
    }

    if (success) {
        labels[INSTANCE_NAME] = new Label(instance, labels[SERVICE_NAME], true);

        success = labels[INSTANCE_NAME]->getSize() == instance.length();
    }

    this->port = port;

    return success;
}

bool MDNS::addTXTEntry(String key, String value) {
    return txtData->addEntry(key, value);
}

bool MDNS::begin() {
    // Wait for WiFi to connect
    while (!WiFi.ready()) {
    }

    udp->begin(MDNS_PORT);
    IPAddress mdnsdomain(224,0,0,251); // mDNS broadcast domain
    udp->joinMulticast(mdnsdomain); // join mDNS broadcast domain

    // TODO: Probing + announcing

    return true;
}

int MDNS::processQueries() {
    uint16_t n = udp->parsePacket();

    uint8_t responses = 0;
    uint16_t remotePort = 0;

    if (n > 0) {
    	logDebug("%d:Got UDP Packet", millis());
        buffer->read(udp);
        
        remotePort = udp->remotePort();

        udp->flush();
        udp->stop();

        if (buffer->available() >= 12) {
            /*uint16_t id = */buffer->readUInt16();
            uint16_t flags = buffer->readUInt16();
            uint16_t qdcount = buffer->readUInt16();
            /*uint16_t ancount = */buffer->readUInt16();
            /*uint16_t nscount = */buffer->readUInt16();
            /*uint16_t arcount = */buffer->readUInt16();

            // Spark.publish("splunk/mdns/processQueries", "Query " + String(flags, HEX) + " " + String(qdcount, HEX), 60, PRIVATE);
            logDebug("splunk/mdns/processQueries Query flags:0x%x qdcount:0x%x",flags,qdcount);

            if ((flags & 0x8000) == 0) {
                while (qdcount-- > 0) {
                    int8_t matchedName = matcher->match(buffer);
                    
                    if (buffer->available() >= 4) {
                        uint16_t type = buffer->readUInt16();
                        uint16_t cls = buffer->readUInt16();

                        // Spark.publish("splunk/mdns/processQueries", "Query " + matcher->getLastName() + " " + String(type, HEX) + " " + String(cls, HEX) + " " + matchedName, 60, PRIVATE);
                        char strbuf[30];
                        String lastName=matcher->getLastName();
                        lastName.toCharArray(strbuf, 30);
                        logDebug("splunk/mdns/processQueries Query lastname:%s type:0x%x, cls:0x%x matchedName:%d",strbuf,type,cls,matchedName);

                        if (matchedName >= 0) {
    
                            switch (matchedName) {
                                case HOST_NAME:
                                    // TODO: Negative response for AAAA
    
                                    if (type == A_TYPE || type == ANY_TYPE) {
                                        responses |= A_AN_FLAG;
                                    }
                                    break;
    
                                case SERVICE_NAME:
                                    if (type == PTR_TYPE || type == ANY_TYPE) {
                                        responses |= PTR_AN_FLAG | SRV_AD_FLAG | TXT_AD_FLAG | A_AD_FLAG;
                                    }
                                    break;
    
                                case INSTANCE_NAME:
                                    if (type == SRV_TYPE) {
                                        responses |= SRV_AN_FLAG | A_AD_FLAG;
                                    } else if (type == TXT_TYPE) {
                                        responses |= TXT_AN_FLAG;
                                    } else if (type == ANY_TYPE) {
                                        responses |= SRV_AN_FLAG | TXT_AN_FLAG | A_AD_FLAG;
                                    }
                                    break;
    
                                default:
                                    break;
                            }
                        } else if (matchedName == BUFFER_UNDERFLOW) {
                            qdcount = 0;
                        }
                    }
                }
            }
        }

        buffer->clear();

        if (responses > 0) {
            // Reset output offsets for name compression
            for (uint8_t i = 0; i < NAME_COUNT; i++) {
                labels[i]->reset();
            }

            // Don't send additional responses where we are already sending answers
            responses &= ~(responses << 4);

            // Counting trick
            uint8_t counts = (responses & 0x55) + ((responses >> 1) & 0x55);
            counts = (counts & 0x33) + ((counts >> 2) & 0x33);

            // Spark.publish("splunk/mdns/processQueries", "Response " + String(counts & 0x3) + " " + String(counts >> 4) + " " + String(responses, BIN), 60, PRIVATE);

            buffer->writeUInt16(0x0);
            buffer->writeUInt16(0x8400);
            buffer->writeUInt16(0x0);
            buffer->writeUInt16(counts & 0x3);
            buffer->writeUInt16(0x0);
            buffer->writeUInt16(counts >> 4);

            while (responses > 0) {
                if ((responses & A_AN_FLAG) != 0) {
                    writeARecord();
                }
                if ((responses & PTR_AN_FLAG) != 0) {
                    writePTRRecord();
                }
                if ((responses & SRV_AN_FLAG) != 0) {
                    writeSRVRecord();
                }
                if ((responses & TXT_AN_FLAG) != 0) {
                    writeTXTRecord();
                }

                responses >>= 4;
            }
        }

        udp->begin(MDNS_PORT);

        if (buffer->available() > 0) {
            udp->beginPacket(IPAddress(224, 0, 0, 251), remotePort);

            buffer->write(udp);

            udp->endPacket();
        }
    }

    return n;
}

void MDNS::writeARecord() {
    writeRecord(HOST_NAME, A_TYPE, TTL_2MIN);
    buffer->writeUInt16(4);
    IPAddress ip = WiFi.localIP();
    for (int i = 0; i < IP_SIZE; i++) {
        buffer->writeUInt8(ip[i]);
    }
}

void MDNS::writePTRRecord() {
    writeRecord(SERVICE_NAME, PTR_TYPE, TTL_75MIN);
    buffer->writeUInt16(labels[INSTANCE_NAME]->getWriteSize());
    labels[INSTANCE_NAME]->write(buffer);
}

void MDNS::writeSRVRecord() {
    writeRecord(INSTANCE_NAME, SRV_TYPE, TTL_2MIN);
    buffer->writeUInt16(6 + labels[HOST_NAME]->getWriteSize());
    buffer->writeUInt16(0);
    buffer->writeUInt16(0);
    buffer->writeUInt16(port);
    labels[HOST_NAME]->write(buffer);
}

void MDNS::writeTXTRecord() {
    writeRecord(INSTANCE_NAME, TXT_TYPE, TTL_75MIN);
    txtData->write(buffer);
}

void MDNS::writeRecord(uint8_t nameIndex, uint16_t type, uint32_t ttl) {
    labels[nameIndex]->write(buffer);
    buffer->writeUInt16(type);
    buffer->writeUInt16(IN_CLASS);
    buffer->writeUInt32(ttl);
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
