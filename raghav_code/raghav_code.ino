#define NUM_SENSORS 5
#define LED 11
#include <SoftwareSerial.h>
#define NUM_GESTURES 7
#define BUTTON 7
#define HIST_LENGTH 50
#define NUM_ACTIONS 1
#define MAX_ACTION_LENGTH 3

int val = 0;
int op = 0;
int prevop = 0;
boolean isDebug = true;
String data = "";
SoftwareSerial espSerial(10, 11);

int gesture[NUM_SENSORS] = {0};

const int threshold[3] PROGMEM= {60,120,500}; // 3 levels of bending for each finger
const int flexPins[NUM_SENSORS] = {A0,A1,A2,A3,A4};
int ranges[NUM_SENSORS][2] = {{710,850},{730,1100},{550,860},{500,960},{1000,1330}};  //default values
const int gestures[NUM_GESTURES][NUM_SENSORS] PROGMEM = {
{0,0,0,0,0}, //palm outstretched
{2,2,2,2,2}, //fist
{2,0,0,0,0}, //thumb
{0,2,0,0,0}, //index 
{0,0,2,0,0}, //middle
{0,0,0,2,0}, //ring
{0,0,0,0,2} //little
};

const String gesture_names[NUM_GESTURES] = {
 "palm outstretched",
 "fist",
 "thumb",
 "index", 
 "middle",
 "ring",
"little"
};

uint8_t history[HIST_LENGTH] = {0}; // Reduced size

int index = 1;
String curAction;
const int action_length[NUM_ACTIONS] = {3};
String action_meaning[NUM_ACTIONS] = {"making a fist"};

int action[NUM_ACTIONS][MAX_ACTION_LENGTH] = {0}; // Static allocation

void setupActions() {
  int temp[3] = {0, 1, 0}; // Example data
  for (int i = 0; i < action_length[0]; i++) {
    action[0][i] = temp[i];
  }
}


void calibrateFlex(){
  Serial.println("Started calibration");
  for(int i = 0;i<NUM_SENSORS;i++){  //calibrate each sensor one by one
    int minVal = 1023;
    int maxVal = 0;
    Serial.print("Calibrating for finger: ");
    Serial.println(i);
    for(int j = 0;j<1000;j++){
      val = analogRead(flexPins[i]);
      minVal = min(minVal,val);
      maxVal = max(maxVal,val);
      delay(1);
    }
    ranges[i][0] = minVal;
    ranges[i][1] = maxVal;
  }
}

void readSensors(){
  
  for(int i = 0;i < NUM_SENSORS;i++){
    val = analogRead(flexPins[i]);
    int base = ranges[i][0];
    int ceiling = ranges[i][1];
    op = map(val,base,ceiling,0,180);

    // if(isDebug){
    //   Serial.print(val); //raw value
    //   Serial.print(",");
    //   Serial.print(op); //mapped value
    //   Serial.print(",");
    // }

    for(int j = 0;j < 3;j++){
      if(op < threshold[j]){
        gesture[i] = j;
        Serial.print(gesture[j]);
        Serial.print(",");
        break;
      }
    }
    
  }
  Serial.println();
  delay(100);
}

String processData(){
  data = "";
  readSensors();
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

void detectGesture(){
  for(int i = 0;i<NUM_GESTURES;i++){
    bool valid = true;
    for(int j = 0;j<NUM_SENSORS;j++){
      if(gesture[j]!=gestures[i][j]){
        Serial.println("No gesture " + String(gesture[j]) + " " + String(pgm_read_word(&gestures[i][j])));
        valid = false;
        break;
      }
      Serial.print(j +" Sensor correct for gesture "+ gesture_names[i] + " ");
    }
    if(valid){
      for (int j = 0; j < NUM_SENSORS; j++) {
        gesture[j] = pgm_read_word(&gestures[i][j]);
      }
      Serial.println(gesture_names[i] + "is detected");
      break;
    }
    
  }
  memset(gesture, 0, sizeof(gesture));
}

bool cmpGestures(int* gesture1,int* gesture2){
  for(int i = 0;i<NUM_SENSORS;i++){
    if(gesture1[i]!=gesture2[i]){
      return false;
    }
  }
  return true;
}

String detectAction(){
  bool map[NUM_ACTIONS];
  for(int i = 0;i<NUM_ACTIONS;i++){
    map[i] = true;
  }
  for(int i = 0;i<MAX_ACTION_LENGTH;i++){
    for(int j = 0;j<NUM_ACTIONS;j++){
      if(action_length[j] <= i and map[j]){
        if(history[index-i] != action[j][i]){ //will cause an error when MAX_ACTION_LENGTH > index (3 > 1) 
          map[j] = false;
        }
      }
    }
  }
  
  for(int i = 0;i<NUM_ACTIONS;i++){
    if(map[i]){
      return action_meaning[i];
    }
  }
  return "";
}


void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  espSerial.begin(9600);
  calibrateFlex();
  setupActions();
  
}

void loop() {
  readSensors();
  detectGesture();
  delay(1000);
  if(gesture == nullptr){
    return;
  }
  if(cmpGestures(gesture,history[index-1])){
    return;
  }else{
    if (index >= HIST_LENGTH) index = 0; // Reset to prevent overflow
    for (int i = 0; i < NUM_SENSORS; i++) {
    history[index * NUM_SENSORS + i] = gesture[i];
}

  }
  curAction = detectAction();
  if(curAction == NULL){
    return;
  }
  
  espSerial.println(curAction);
  Serial.println(data);
}

