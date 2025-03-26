#define NUM_SENSORS 5
#define LED 11
#include <SoftwareSerial.h>
#define NUM_GESTURES 7
#define BUTTON 7
#define HIST_LENGTH 50
#define NUM_ACTIONS 1
#define MAX_ACTION_LENGTH 3

int val = 0;
boolean isDebug = true;
String data = "";
SoftwareSerial espSerial(10, 11);

int gesture[NUM_SENSORS] = {0};

const int threshold[3] PROGMEM = {60, 120, 500};
const int flexPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};
int ranges[NUM_SENSORS][2] = {{710, 850}, {730, 1100}, {550, 860}, {500, 960}, {1000, 1330}};
const int gestures[NUM_GESTURES][NUM_SENSORS] PROGMEM = {
  {0, 0, 0, 0, 0},
  {2, 2, 2, 2, 2},
  {2, 0, 0, 0, 0},
  {0, 2, 0, 0, 0},
  {0, 0, 2, 0, 0},
  {0, 0, 0, 2, 0},
  {0, 0, 0, 0, 2}
};

const String gesture_names[NUM_GESTURES] = {
  "palm outstretched", "fist", "thumb", "index", "middle", "ring", "little"
};

uint8_t history[HIST_LENGTH][NUM_SENSORS] = {0};
int index = 0;

void calibrateFlex() {
  Serial.println("Started calibration");
  for (int i = 0; i < NUM_SENSORS; i++) {
    int minVal = 1023;
    int maxVal = 0;
    Serial.print("Calibrating for finger: ");
    Serial.println(i);
    for (int j = 0; j < 1000; j++) {
      val = analogRead(flexPins[i]);
      minVal = min(minVal, val);
      maxVal = max(maxVal, val);
      delay(1);
    }
    ranges[i][0] = minVal;
    ranges[i][1] = maxVal;
  }
}

void readSensors() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    val = analogRead(flexPins[i]);
    int base = ranges[i][0];
    int ceiling = ranges[i][1];
    int op = map(val, base, ceiling, 0, 180);


    for (int j = 0; j < 3; j++) {
      if (op < threshold[j]) {
        gesture[i] = j;
        Serial.print(String(gesture[i])+",");
        break;
      }
    }
  }
  Serial.println();
}

void detectGesture() {
  for (int i = 0; i < NUM_GESTURES; i++) {
    bool valid = true;
    for (int j = 0; j < NUM_SENSORS; j++) {
      if (gesture[j] != pgm_read_word(&gestures[i][j])) {
        valid = false;
        break;
      }
    }
    if (valid) {
      Serial.println(gesture_names[i] + " is detected");
      return;
    }
  }
  Serial.println("No gesture detected");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  espSerial.begin(9600);
  calibrateFlex();
}

void loop() {
  readSensors();
  detectGesture();
  delay(500);

  for (int i = 0; i < NUM_SENSORS; i++) {
    history[index][i] = gesture[i];
  }
  index = (index + 1) % HIST_LENGTH;
} 
