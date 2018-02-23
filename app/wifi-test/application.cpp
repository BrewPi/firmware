#include "application.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
SerialLogHandler traceLog(LOG_LEVEL_TRACE);

static TCPServer tcpServer = TCPServer(6666);
static TCPClient tcpClient;

enum tcp_state_enum {
    RUNNING_FINE,
    OH_NO_ITS_BORKED_PARTICLE,
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
        SINGLE_THREADED_BLOCK(){
            tcp_state = tcp_state_enum::OH_NO_ITS_BORKED_PARTICLE;
        }
        break;
    case network_status_connected:
        SINGLE_THREADED_BLOCK(){
            tcp_state = tcp_state_enum::ALLOWED_TO_RESTART;
        }
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
    pinMode(D7, OUTPUT);

    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);

    System.on(network_status, handle_network_events);
    System.on(cloud_status, handle_cloud_events);
    tcp_state = tcp_state_enum::ALLOWED_TO_RESTART;
}

void loop() {
    static uint32_t last_update = millis();

    switch(tcp_state){
        case tcp_state_enum::RUNNING_FINE:
            break;
        case tcp_state_enum::OH_NO_ITS_BORKED_PARTICLE:
            Serial.print("Stopping TCP\n");
            SINGLE_THREADED_BLOCK(){
                tcp_state = tcp_state_enum::STOPPED;
            }
            stopTcp();
            break;
        case tcp_state_enum::ALLOWED_TO_RESTART:
            Serial.print("Restarting TCP\n");
            startTcp();
            SINGLE_THREADED_BLOCK(){
                tcp_state = tcp_state_enum::RUNNING_FINE; // assume it is running fine afterwards
            }
            break;

    }

    if(tcp_state == tcp_state_enum::RUNNING_FINE){
        if(tcpClient.status()){
            while (tcpClient.available() > 0) {
                char inByte = tcpClient.read();
                switch(inByte){
                case ' ':
                case '\n':
                case '\r':
                    break;
                case 't':
                    size_t result = tcpClient.write("toc"); // send toc back over tcp
                    Serial.printf("hw->py: toc (%d bytes sent) \n", result); // confirm toc sent over tcp
                }
                Serial.printf("py->hw: %c\n", inByte); // confirm character received from tcp
            }
        }
        else{
            tcpClient.stop();
            // listen for a new client
            TCPClient newClient = tcpServer.available();
            if(newClient) {
               Serial.print("New TCP client\n");
               tcpClient.stop();
               tcpClient = newClient;
            }
        }
    }

    // print status on serial every second
    if ( millis() - last_update > 1000UL ) {
        last_update = millis();
        bool wifiReady = WiFi.ready();
        IPAddress ip = WiFi.localIP();
        Serial.printf("WiFi.ready(): %d\t\tIP: %d.%d.%d.%d\t\tTCP client connected: %d\n", wifiReady, ip[0],ip[1],ip[2],ip[3], tcpClient.connected());
    }

    uint8_t ledOn = millis() / 300 % 2;
    digitalWrite(D7, ledOn);
}
