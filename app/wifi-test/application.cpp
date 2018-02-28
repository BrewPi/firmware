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
    static int cloud_connection_tries = 0;
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
    tcp_state = tcp_state_enum::ALLOWED_TO_RESTART;
}

void loop() {
    static system_tick_t last_update = millis();
    static system_tick_t lastLedToggle = millis();

    if(WiFi.RSSI() >= 0){
        // WiFi is in error state, stop TCP server
        if(tcp_state != tcp_state_enum::STOPPED){
            tcp_state = tcp_state_enum::NEEDS_TO_STOP;
        }
    }

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
