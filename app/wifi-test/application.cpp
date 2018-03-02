#include "application.h"
#include <inttypes.h>

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
SerialLogHandler traceLog(LOG_LEVEL_TRACE);

static TCPServer tcpServer = TCPServer(6666);
static TCPClient tcpClient;
bool tcpServerRunning;

// Toggle LED pin to see that application loop is not blocked.
// You could use D7 when testing with a Photon.
// I'm using another pin here, because D7 is one of the SWD pins used by the debugger
const int LED_PIN = P1S0;


void stopTcp(){
    tcpServer.stop();
    tcpClient.stop();
    tcpServerRunning = false;
    Serial.print("TCP server stopped\n");
}

void startTcp(){
    tcpServer.begin();
    tcpServerRunning = true;
    Serial.print("TCP server started\n");
}


void handle_network_events(system_event_t event, int param){
    switch(param){
    case network_status_powering_on:
        break;
    case network_status_on:
        break;
    case network_status_powering_off:
        break;
    case network_status_off:
        break;
    case network_status_connecting:
        break;
    case network_status_connected:
        break;
    default:
        break;
    }
    Serial.printf("network event %d\n", param);
}

void handle_cloud_events(system_event_t event, int param){
    switch(param){
    case cloud_status_connecting:
        break;
    case cloud_status_connected:
        break;
    case cloud_status_disconnecting:
        break;
    case cloud_status_disconnected:
        break;
    default:
        break;
    }
    Serial.printf("cloud event %d\n", param);
}

// return time that has passed since timeStamp, take overflow into account
system_tick_t timeSince(system_tick_t previousTime) {
    system_tick_t currentTime = millis();
    if(currentTime>=previousTime){
        return currentTime - previousTime;
    }
    else{
        // overflow has occurred
        return (currentTime + 1440000) - (previousTime +1440000); // add a day to both for calculation
    }
}


void setup() {
    System.set(SYSTEM_CONFIG_SOFTAP_PREFIX, "BREWPI");
    System.disable(SYSTEM_FLAG_RESET_NETWORK_ON_CLOUD_ERRORS);
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);


    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
    Particle.connect();

    System.on(network_status, handle_network_events);
    System.on(cloud_status, handle_cloud_events);
}

void loop() {
    static system_tick_t last_update = millis();
    static system_tick_t lastLedToggle = millis();

    if(!WiFi.ready()){
        // WiFi is in error state, stop TCP server
        if(tcpServerRunning){
            stopTcp();
        }
    }
    else{
    	if(!tcpServerRunning){
			startTcp();
		}
    }

    if(tcpServerRunning){
        if(tcpClient.status()){
            bool noErrors = true;
            while (noErrors && tcpClient.available() > 0) {
                int received = tcpClient.read();
                switch(received){
                case ' ':
                case '\n':
                case '\r':
                    break;
                case 't':
                {
                    size_t result = tcpClient.write("toc"); // send toc back over tcp
                    Serial.printf("hw->py: toc (%d bytes sent) \n", result); // confirm toc sent over tcp
                }
                    break;
                default:
                    if(received < 0){
                        Serial.printf("Receive error: %d\n", received); // confirm toc sent over tcp
                        noErrors = false;
                    }
                    else{
                        Serial.printf("py->hw: %c\n", received); // confirm character received from tcp
                    }
                    break;
                }
            }
        }
        // listen for a new client, drop the old one if a new client arrives
        TCPClient newClient = tcpServer.available();
        if(newClient) {
           Serial.print("New TCP client\n");
           tcpClient.write("New TCP client arrived, dropping you."); // stop old client
           tcpClient.stop(); // stop old client
           tcpClient = newClient;
        }
    }

    // print status on serial every second
    if(timeSince(last_update) >= 1000UL ) {
        last_update = millis();
        bool wifiReady = WiFi.ready();
        IPAddress ip = WiFi.localIP();
        int signal = WiFi.RSSI();
        int clientConnected = tcpClient.connected();
        Serial.printf(
                "WiFi.ready(): %d\t\t"
                "IP: %d.%d.%d.%d\t\t"
                "RSSI: %d\t\t"
                "TCP client connected: %d\t\t"
                "millis(): %" PRIu32 "\n",
                wifiReady,
                ip[0],ip[1],ip[2],ip[3],
                signal,
                clientConnected,
                last_update);
    }

    if(timeSince(lastLedToggle) >= 200UL ) {
        lastLedToggle = millis();
        static bool ledOn = true;
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn);
    }
    Particle.process();
}
