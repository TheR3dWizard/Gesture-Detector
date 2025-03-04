#define NUM_SENSORS 5
#define LED 11
#include <SoftwareSerial.h>


int val = 0;
int op = 0;
int prevop = 0;
boolean isDebug = false;
String data = "";
SoftwareSerial espSerial(10, 11);

const int threshold[3] = {60,120,500}; // 3 levels of bending for each finger
const int flexPins[NUM_SENSORS] = {A0,A1,A2,A3,A4};
const int ranges[NUM_SENSORS][2] = {{710,850},{530,900},{490,800},{340,800},{300,630}};  //base + high

void setup() {
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  Serial.begin(9600);
  espSerial.begin(9600);
}

void loop() {
  data = "";
  for(int i = 0;i < NUM_SENSORS;i++){
    val = analogRead(flexPins[i]);
    int base = ranges[i][0];
    int offset = ranges[i][1];
    op = map(val,base,offset,0,180);

    if(isDebug){
      data += String(val) + ","+String(op)+",";
      Serial.print(val); //raw value
      Serial.print(",");
      Serial.print(op); //mapped value
    }
    for(int i = 0;i < 3;i++){
      if(op < threshold[i]){
        data += String(i);
        Serial.print(i);  //gesture
        break;
      }
    }
    data += ",";
    Serial.print(",");
  }
  espSerial.println(data);
  Serial.println();
  delay(100);
}
