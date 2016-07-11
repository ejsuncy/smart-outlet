#include <Arduino.h>
#include <pins_arduino.h>
#define OUTLET_1 D3
#define OUTLET_2 D1

void setup() {
    pinMode(OUTLET_1, OUTPUT);  // initialize relay signal GPIO pins as output
    pinMode(OUTLET_2, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(OUTLET_1, LOW); // Start with both outlets OFF
    digitalWrite(OUTLET_2, LOW);
    digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(OUTLET_1, HIGH);
    delay(1000);
    digitalWrite(OUTLET_2, HIGH);
    delay(1000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
    digitalWrite(OUTLET_1, LOW);
    delay(1000);
    digitalWrite(OUTLET_2, LOW);
    delay(1000);
}
