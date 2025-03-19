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
        readval.trim(); // Trim removes spaces & newlines
        
        if (readval.length() > 0) {
            Serial.print("Received Data: ");
            Serial.println(readval);
            
            int index = 0;
            int start = 0;
            int end = readval.indexOf(',');

            // Initialize values to 0 (prevents uninitialized access)
            memset(values, 0, sizeof(values));

            while (end != -1 && index < MAX_VALUES) {
                values[index] = readval.substring(start, end).toInt();
                index++;
                start = end + 1;
                end = readval.indexOf(',', start);
            }

            // Last value (after the last comma)
            if (index < MAX_VALUES) {
                values[index] = readval.substring(start).toInt();
            }

            // Check WiFi before sending data
            if (WiFi.status() == WL_CONNECTED) {
                for (int i = 0; i < MAX_VALUES; i++) {
                    ThingSpeak.writeField(myChannelNumber, i + 1, values[i], myWriteAPIKey);
                    Serial.print("Uploaded Value: ");
                    Serial.println(values[i]);
                    delay(15000); // Respect ThingSpeak rate limit
                }
            } else {
                Serial.println("WiFi Disconnected! Skipping upload.");
            }
        }
    }
}
