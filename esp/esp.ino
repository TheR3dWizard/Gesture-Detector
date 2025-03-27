#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>

#define LED 3
#define MAX_VALUES 5 // Define the number of expected values

SoftwareSerial mySerial(14, 5);  // RX = GPI14 (D5), TX = GPIO5 (D1)
const char* ssid = "MotoG6";
const char* password = "12345678";
WiFiClient client;

unsigned long myChannelNumber = 2862615; // ThingSpeak channel number
const char* myWriteAPIKey = "RORN8653YJP4HBEX"; // ThingSpeak API key

int values[MAX_VALUES];  // Store parsed values
char data[100]; // Buffer for incoming data

void setup() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    Serial.begin(9600);
    mySerial.begin(9600);  

    delay(500);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected to Wi-Fi");
    Serial.println(WiFi.localIP()); // Print the IP address

    ThingSpeak.begin(client);
    Serial.println("ESP8266 Ready");
}

void loop() {
    if (mySerial.available()) {
        String readval = mySerial.readStringUntil('\n');
        readval.trim(); 
        
        if (readval.length() > 0) {
            Serial.print("Received Data: ");
            Serial.println(readval);
            

            // Check WiFi before sending data
            if (WiFi.status() == WL_CONNECTED) {
              ThingSpeak.writeField(myChannelNumber, 6, readval, myWriteAPIKey);  
            } else {
                Serial.println("WiFi Disconnected! Skipping upload.");
            }
        }
    }
}
