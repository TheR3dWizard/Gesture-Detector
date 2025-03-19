#define NUM_SENSORS 5
#define LED 11
#include <SoftwareSerial.h>
#define NUM_GESTURES 7
#define BUTTON 7
#define HIST_LENGTH 1000
#define NUM_ACTIONS 1
#define MAX_ACTION_LENGTH 3

int val = 0;
int op = 0;
int prevop = 0;
boolean isDebug = false;
String data = "";
SoftwareSerial espSerial(10, 11);

int* gesture;
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

int history[HIST_LENGTH] = {0};
int index = 1;
String curAction;
const int** action;
const int action_length[NUM_ACTIONS] = {3};
String action_meaning[NUM_ACTIONS] = {"making a fist"};

void setupActions(){
  for(int i = 0;i<NUM_ACTIONS;i++){
    action[i] = (int*)malloc(action_length[i]*sizeof(int));
  }
  int temp[3] = {0,1,0};
  memcpy((int*)action[0],temp,3*sizeof(int));
}

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

void readSensors(){
  
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
  readSensors();
  for(int i = 0;i<NUM_GESTURES;i++){
    bool valid = true;
    for(int j = 0;j<NUM_SENSORS;j++){
      if(gesture[j]!=gestures[i][j]){
        valid = false;
        break;
      }
      if(valid){
        gesture = gestures[i];
      }
    }
  }
  gesture = nullptr;
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
  return NULL;
}


void setup() {
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  Serial.begin(9600);
  espSerial.begin(9600);
  calibrateFlex();
  setupActions();
}

void loop() {
  reset();
  detectGesture();
  if(gesture == nullptr){
    return;
  }
  if(cmpGestures(gesture,history[index-1])){
    return;
  }else{
    history[index++] = gesture;
  }
  curAction = detectAction();
  if(curAction == NULL){
    return;
  }
  espSerial.println(curAction);
  Serial.println(data);
  delay(100);
}
