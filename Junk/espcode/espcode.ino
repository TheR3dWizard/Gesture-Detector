#include <ESP8266WiFi.h>
#define LED 3

const char* ssid = "MotoG6";
const char* password = "12345678";
String data = "";

void setup() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    Serial.begin(115200);
    Serial1.begin(9600);  // ESP8266 UART1 for receiving from Arduino

    delay(500);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected to Wi-Fi");
    Serial.println(WiFi.localIP()); // Print the IP address
}

void loop() {
    // Serial.println("Checking for messages");
    while (Serial1.available()) {
      if (Serial1.available()) {
          String data = Serial1.readStringUntil('\n');
          Serial.print("ESP Received: ");
          Serial.println(data);
        }
    }
    while (Serial.available()) {
      if (Serial.available()) {
          String data = Serial.readStringUntil('\n');
          Serial.print("ESP Received: ");
          Serial.println(data);
        }
    }
}



