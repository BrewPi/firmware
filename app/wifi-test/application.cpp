#include "application.h"
#include <inttypes.h>

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
SerialLogHandler traceLog(LOG_LEVEL_TRACE);

static TCPServer tcpServer = TCPServer(6666);
static TCPClient tcpClient;


// Toggle LED pin to see that application loop is not blocked.
// You could use D7 when testing with a Photon.
// I'm using another pin here, because D7 is one of the SWD pins used by the debugger
const int LED_PIN = P1S0;

enum tcp_state_enum {
    RUNNING_FINE,
    NEEDS_TO_STOP,
    STOPPED,
    ALLOWED_TO_RESTART,
};

volatile uint8_t tcp_state;

void stopTcp(){
    tcpServer.stop();
    tcpClient.stop();
    Serial.print("TCP server stopped\n");
}

void startTcp(){
    tcpServer.begin();
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
        // set a flag to restart the TCP server, outside of the system thread
        // restarting it here doesn't work
        // leaving it running doesn't work
        if(tcp_state != tcp_state_enum::STOPPED){
            tcp_state = tcp_state_enum::NEEDS_TO_STOP;
        }
        break;
    case network_status_connected:
        tcp_state = tcp_state_enum::ALLOWED_TO_RESTART;
        break;
    default:
        break;
    }
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
}


void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);

    System.on(network_status, handle_network_events);
    System.on(cloud_status, handle_cloud_events);
    tcp_state = tcp_state_enum::ALLOWED_TO_RESTART;
}

void loop() {
    static uint32_t last_update = millis();
    static uint32_t lastLedToggle = millis();

    switch(tcp_state){
        case tcp_state_enum::RUNNING_FINE:
            if(!WiFi.ready()){
                // check in case WiFi is not ready but the system events didn't trigger the correct state
                tcp_state = tcp_state_enum::NEEDS_TO_STOP;
            }
            break;
        case tcp_state_enum::NEEDS_TO_STOP:
            Serial.print("Stopping TCP\n");
            stopTcp();
            tcp_state = tcp_state_enum::STOPPED;
            break;
        case tcp_state_enum::STOPPED:
            break;
        case tcp_state_enum::ALLOWED_TO_RESTART:
            Serial.print("Restarting TCP\n");
            startTcp();
            tcp_state = tcp_state_enum::RUNNING_FINE; // assume it is running fine afterwards
            break;

    }

    if(tcp_state == tcp_state_enum::RUNNING_FINE){
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
           tcpClient.stop(); // stop old client
           tcpClient = newClient;
        }
    }

    // print status on serial every second
    if ( millis() - last_update > 1000UL ) {
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


        /* When the signal gets below -80, the P1 will stop responding to TCP.
         * - It still has an IP
         * - It still has WiFi.ready() returning 1.
         * - The LED is still breathing green.
         *
         * There is one symptom that something is wrong though:
         * When TCP stopped working, the P1 would return RSSI 2.
         *
         * This seems to be a failed to get RSSI timeout:
         *
            int8_t WiFiClass::RSSI() {
                if (!network_ready(*this, 0, NULL))
                    return 0;

                system_tick_t _functionStart = millis();
                while ((millis() - _functionStart) < 1000) {
                    int rv = wlan_connected_rssi();
                    if (rv != 0)
                        return (rv);
                }
                return (2);
            }
         *
         * In an attempt to fix this, I tried disconnecting and reconnecting WiFi.
         * But WiFi.disconnect() gives a stack overflow SOS (13 blinks).
         */


        if(signal == 2){
            Serial.print("\n\n\nWiFi is in ERROR state. Resetting WiFi\n\n\n");
            WiFi.disconnect();
            WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
        }
    }



    if ( millis() - lastLedToggle > 200UL ) {
        static bool ledOn = true;
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn);
        lastLedToggle = millis();
    }
}
