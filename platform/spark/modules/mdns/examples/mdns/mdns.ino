#include "MDNS.h"

#include <vector>

#define HTTP_PORT 80
#define ALT_HTTP_PORT 8080

MDNS mdns;

TCPServer server = TCPServer(HTTP_PORT);
TCPServer altServer = TCPServer(ALT_HTTP_PORT);

void setup() {
  server.begin();
  altServer.begin();

  std::vector<String> subServices;

  subServices.push_back("printer");

  bool success = mdns.setHostname("core-1");

  if (success) {
    success = mdns.addService("tcp", "customhttp", HTTP_PORT, "Core 1", subServices);
  }

  mdns.addTXTEntry("normal");

  if (success) {
    success = mdns.addService("tcp", "http", ALT_HTTP_PORT, "Core alt");
  }

  mdns.addTXTEntry("alt");

  if (success) {
    // Announce on startup
    success = mdns.begin(true);
  }
}

void loop() {
  mdns.processQueries();

  TCPClient client = server.available();

  if (client){
    while (client.read() != -1);

    client.write("HTTP/1.1 200 Ok\n\n<html><body><h1>Ok!</h1></body></html>\n\n");
    client.flush();
    delay(5);
    client.stop();
  }

  TCPClient altClient = altServer.available();

  if (altClient){
    while (altClient.read() != -1);

    altClient.write("HTTP/1.1 200 Ok\n\n<html><body><h1>Alternative port ok!</h1></body></html>\n\n");
    altClient.flush();
    delay(5);
    altClient.stop();
  }
}
