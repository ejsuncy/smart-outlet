#include <Arduino.h>
#include <pins_arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSockets.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define OUTLET_1 D3
#define OUTLET_2 D1

static const uint8_t D1 = 5;
static const uint8_t D3 = 0;

static const char *DNS_NAME = "esp8266";
static const char *AP_NAME = "ESP8266 Smart Outlet";
static const char *AP_PASS = "passwordpassword";
static const unsigned long SERIAL_DEBUG_BAUD= 115200;

WebSocketsServer ws = WebSocketsServer(81);

void turnOn(uint8_t outlet) {
    pinMode(outlet, HIGH);
}

void turnOff(uint8_t outlet) {
    pinMode(outlet, LOW);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length){

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Websocket Disconnected...");
            break;

        case WStype_CONNECTED:
            Serial.println("Websocket Connected!");
            break;
        case WStype_TEXT:
            String text = String((char *) &payload[0]);
            Serial.println("Received text:");
            Serial.println(text);

            // process the text however you want
            if (text.startsWith("on:")) {
                ws.sendTXT(num, "ack: on");
                String outletNumber = text.substring(3, 4);

                if (outletNumber.equals("1")) {
                    turnOn(OUTLET_1);
                }
                else if (outletNumber.equals("2")) {
                    turnOn(OUTLET_2);
                }
            }
            else if (text.startsWith("off:")) {
                ws.sendTXT(num, "ack: off");
                String outletNumber = text.substring(4, 5);

                if (outletNumber.equals("1")) {
                    turnOff(OUTLET_1);
                }
                else if (outletNumber.equals("2")) {
                    turnOff(OUTLET_2);
                }
            }

            break;
    }
}

String statusString(wl_status_t status) {
    switch(status) {
        case WL_CONNECTED:
            return "WL_CONNECTED";
        case WL_NO_SSID_AVAIL:
            return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
            return "WL_CONNECT_FAILED";
        case WL_IDLE_STATUS:
            return "WL_IDLE_STATUS";
        case WL_DISCONNECTED:
            return "WL_DISCONNECTED";
        default:
            return "UNKNOWN";
    }
}

void setupPinModes() {
    pinMode(OUTLET_1, OUTPUT);  // initialize relay signal GPIO pins as output
    pinMode(OUTLET_2, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(OUTLET_1, LOW); // Start with both outlets OFF
    digitalWrite(OUTLET_2, LOW);
    digitalWrite(BUILTIN_LED, LOW);
}

void setupSerialDebugging() {
    Serial.begin(SERIAL_DEBUG_BAUD);
    delay(5000);
    Serial.println("Serial Debugging setup complete");
}

void setupWifi() {
    WiFiManager wifiManager;
    wifiManager.autoConnect(AP_NAME,AP_PASS);
    Serial.println("Wifi Manager Setup complete");
}

void setupDNS() {
    while (!MDNS.begin(DNS_NAME)) {
        delay(1000);
    }
    Serial.println("mDNS responder setup complete");
}

void setupWebsocketServer() {
    ws.begin();
    ws.onEvent(webSocketEvent);
    Serial.println("Websocket Server setup complete");
}

void setup() {
    setupPinModes();
    setupSerialDebugging();
    setupWifi();
    setupDNS();
    setupWebsocketServer();
    Serial.println("Listening...");
}


void loop() {
    ws.loop();
}
