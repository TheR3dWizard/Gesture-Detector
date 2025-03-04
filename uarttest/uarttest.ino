#include <SoftwareSerial.h>

SoftwareSerial espSerial(10, 11);  // RX, TX (Connect to ESP8266 TX, RX)

#define LED 13

void setup() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    Serial.begin(9600);      // For debugging
    espSerial.begin(9600);   // UART Communication with ESP8266
}

void loop() {
    String message = "Hello from Arduino!";
    espSerial.println(message);   // Send data to ESP8266

    Serial.println("Sent: " + message);  // Debug print

    delay(1000);  // Wait 1 second
}
