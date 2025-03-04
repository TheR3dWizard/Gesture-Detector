#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

const char* ssid = "MotoG6"; // your Wi-Fi SSID
const char* password = "12345678"; // your Wi-Fi password

WiFiClient client;

unsigned long myChannelNumber = 2862615; // your ThingSpeak channel number
const char * myWriteAPIKey = "RORN8653YJP4HBEX"; // your ThingSpeak Write API key

// Dummy values for flex sensors
int thumbFlex = 180;
int indexFlex = 0;
int middleFlex = 2;
int ringFlex = 1;
int pinkyFlex = 0;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200); // Serial Monitor communication (for debugging)

  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
  Serial.println("Connected to ThingSpeak");
}

void loop() {
  // Print the dummy flex sensor values to Serial Monitor for debugging
  Serial.print("Thumb Flex Value: ");
  Serial.println(thumbFlex);
  Serial.print("Index Flex Value: ");
  Serial.println(indexFlex);
  Serial.print("Middle Flex Value: ");
  Serial.println(middleFlex);
  Serial.print("Ring Flex Value: ");
  Serial.println(ringFlex);
  Serial.print("Pinky Flex Value: ");
  Serial.println(pinkyFlex);

  // Write the dummy data to ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 1, thumbFlex, myWriteAPIKey);  // Field 1 for Thumb
  delay(15000); // Delay reduced to 15 seconds
  ThingSpeak.writeField(myChannelNumber, 2, indexFlex, myWriteAPIKey);  // Field 2 for Index
  delay(15000); // Delay reduced to 15 seconds
  ThingSpeak.writeField(myChannelNumber, 3, middleFlex, myWriteAPIKey); // Field 3 for Middle
  delay(15000); // Delay reduced to 15 seconds
  ThingSpeak.writeField(myChannelNumber, 4, ringFlex, myWriteAPIKey);   // Field 4 for Ring
  delay(15000); // Delay reduced to 15 seconds
  ThingSpeak.writeField(myChannelNumber, 5, pinkyFlex, myWriteAPIKey);  // Field 5 for Pinky
  delay(15000); // Delay reduced to 15 seconds
}