// Borrowed example code from https://gist.github.com/igrr/43d5c52328e955bb6b09

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

const char* ssid = "..........";
const char* password = "...........";

WiFiUDP listener;

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println("");
    Serial.println("OTA test");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    listener.begin(8266);

    Serial.print("Sketch size: ");
    Serial.println(ESP.getSketchSize());
    Serial.print("Free size: ");
    Serial.println(ESP.getFreeSketchSpace());
}

void loop() {
    int cb = listener.parsePacket();
    if (cb) {
        IPAddress remote = listener.remoteIP();
        int cmd  = listener.parseInt();
        int port = listener.parseInt();
        int sz   = listener.parseInt();
        Serial.println("Got packet");
        Serial.printf("%d %d %d\r\n", cmd, port, sz);
        WiFiClient cl;
        if (!cl.connect(remote, port)) {
            Serial.println("failed to connect");
            return;
        }

        listener.stop();

        if (!ESP.updateSketch(cl, sz)) {
            Serial.println("Update failed");
        }
    }

    delay(100);
}

