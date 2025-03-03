#include <ESP8266WiFi.h>

const char* ssid = "HOMENET";
const char* password = "Thanky0u";

const int led = 3;

void setup() {

    pinMode(3,OUTPUT);
    digitalWrite(3,HIGH);

    Serial.begin(115200);
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
}