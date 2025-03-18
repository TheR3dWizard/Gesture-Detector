#define NUM_SENSORS 5
#define LED 11
#include <SoftwareSerial.h>
#define NUM_GESTURES 7
#define BUTTON 7
#define HIST_LENGTH 1000

int val = 0;
int op = 0;
int prevop = 0;
boolean isDebug = false;
String data = "";
SoftwareSerial espSerial(10, 11);

const int threshold[3] = {60,120,500}; // 3 levels of bending for each finger
const int flexPins[NUM_SENSORS] = {A0,A1,A2,A3,A4};
int ranges[NUM_SENSORS][2] = {{710,850},{730,1100},{550,860},{500,960},{1000,1330}};  //default values
const int gestures[NUM_GESTURES][NUM_SENSORS] = {
{0,0,0,0,0}, //palm outstretched
{2,2,2,2,2}, //fist
{2,0,0,0,0}, //thumb
{0,2,0,0,0}, //index 
{0,0,2,0,0}, //middle
{0,0,0,2,0}, //ring
{0,0,0,0,2} //little
};
int history[1000] = {0};
int index = 1;


void calibrateFlex(){
  for(int i = 0;i<NUM_SENSORS;i++){  //calibrate each sensor one by one
    int minVal = 1023;
    int maxVal = 0;
    for(int j = 0;j<1000;){
      val = analogRead(flexPins[i]);
      minVal = min(minVal,val);
      maxVal = max(maxVal,val);
      delay(1);
    }
    ranges[i][0] = minVal;
    ranges[i][1] = maxVal;
  }
}

int* readGesture(){
  int gesture[5];
  for(int i = 0;i < NUM_SENSORS;i++){
    val = analogRead(flexPins[i]);
    int base = ranges[i][0];
    int ceiling = ranges[i][1];
    op = map(val,base,ceiling,0,180);

    if(isDebug){
      Serial.print(val); //raw value
      Serial.print(",");
      Serial.print(op); //mapped value
    }

    for(int i = 0;i < 3;i++){
      if(op < threshold[i]){
        gesture[i] = i;
        break;
      }
    }
  }
  return gesture;
}

String processData(){
  data = "";
  int* gesture = readGesture();
  for(int i = 0;i<NUM_SENSORS;i++){
    data += String(gesture[i]);
    data += ",";
  }
  return data;
}

bool reset(){
  if(digitalRead(BUTTON)){
    calibrateFlex();
  }
}

int* detectGesture(){
  int* gesture = readGesture();
  for(int i = 0;i<NUM_GESTURES;i++){
    bool valid = true;
    for(int j = 0;j<NUM_SENSORS;j++){
      if(gesture[j]!=gestures[i][j]){
        valid = false;
        break;
      }
      if(valid){
        return gestures[i];
      }
    }
  }
  return nullptr;
}

bool cmpGestures(int* gesture1,int* gesture2){
  for(int i = 0;i<NUM_SENSORS;i++){
    if(gesture1[i]!=gesture2[i]){
      return false;
    }
  }
  return true;
}



void setup() {
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  Serial.begin(9600);
  espSerial.begin(9600);
  calibrateFlex();
}

void loop() {
  reset();
  int* gesture = detectGesture();
  if(gesture == nullptr){
    return;
  }
  if(cmpGestures(gesture,history[index-1])){
    return;
  }else{
    history[index++] = gesture;
  }
  action = detectAction();
  espSerial.println(data);
  Serial.println(data);
  delay(100);
}
