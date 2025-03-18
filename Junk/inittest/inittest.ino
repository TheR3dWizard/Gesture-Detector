#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5);  // RX = GPIO4 (D2), TX = GPIO5 (D1)

void setup() {
    Serial.begin(9600);    // Default hardware Serial for debugging
    mySerial.begin(9600);  // SoftwareSerial for communication with Arduino
    Serial.println("ESP8266 Ready");
    
}

void loop() {
    if (mySerial.available()) {
        String data = mySerial.readStringUntil('\n');
        // Serial.print("Received: ");
        Serial.println(data);
        
    }
}
