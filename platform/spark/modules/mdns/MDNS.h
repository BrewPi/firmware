#include "application.h"

#ifndef _INCL_MDNS
#define _INCL_MDNS

#include "Buffer.h"
#include "Label.h"
#include "TxtData.h"

#define MDNS_PORT 5353

#define BUFFER_SIZE 512
#define IP_SIZE 4

#define HOST_NAME 0
#define SERVICE_NAME 1
#define INSTANCE_NAME 2
#define NAME_COUNT 3

#define A_TYPE 0x01
#define PTR_TYPE 0x0c
#define SRV_TYPE 0x21
#define TXT_TYPE 0x10
#define ANY_TYPE 0xFF

#define IN_CLASS 1

#define TTL_2MIN 120
#define TTL_75MIN 4500

#define A_AN_FLAG 0x01
#define PTR_AN_FLAG 0x02
#define SRV_AN_FLAG 0x04
#define TXT_AN_FLAG 0x08
#define A_AD_FLAG 0x10
#define PTR_AD_FLAG 0x20
#define SRV_AD_FLAG 0x40
#define TXT_AD_FLAG 0x80

class MDNS {
    public:

        bool setHostname(String hostname);

        bool setService(String protocol, String service, uint16_t port, String instance);

        bool addTXTEntry(String key, String value);

        bool begin();

        int processQueries();

    private:

        UDP * udp = new UDP();
        Buffer * buffer = new Buffer(BUFFER_SIZE);

        Label * ROOT = new Label("");
        Label * LOCAL = new Label("local", ROOT);
        Label * labels[NAME_COUNT];
        Label::Matcher * matcher = new Label::Matcher(labels, NAME_COUNT);

        uint16_t port;
        TxtData * txtData = new TxtData();

        void writeARecord();
        void writePTRRecord();
        void writeSRVRecord();
        void writeTXTRecord();
        void writeRecord(uint8_t nameIndex, uint16_t type, uint32_t ttl);
        bool isAlphaDigitHyphen(String string);
        bool isNetUnicode(String string);
};

#endif
