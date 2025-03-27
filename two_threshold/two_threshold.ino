#define NUM_SENSORS 5
#define LED 11
#include <SoftwareSerial.h>
#define NUM_GESTURES 8
#define BUTTON 7
#define HIST_LENGTH 50
#define NUM_ACTIONS 1
#define MAX_ACTION_LENGTH 3
#define MAX_SEQUENCE_LENGTH 5

int val = 0;
boolean isDebug = true;
String data = "";
SoftwareSerial espSerial(10, 11);

int gesture[NUM_SENSORS] = {0};

const int threshold[2] PROGMEM = {60,500};
const int flexPins[NUM_SENSORS] = {A0, A1, A2, A3, A4};
int ranges[NUM_SENSORS][2] = {{710, 850}, {730, 1100}, {550, 860}, {500, 960}, {1000, 1330}};
const int gestures[NUM_GESTURES][NUM_SENSORS] PROGMEM = {
  {0, 0, 0, 0, 0},
  {1,1,1,1,1},
  {1, 0, 0, 0, 0},
  {0, 1, 0, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0},
  {0, 0, 0, 0, 1},
  {1,0,1,1,1}
};

const String gesture_names[NUM_GESTURES] = {
  "palm outstretched", "A", "B", "index", "middle", "ring", "little","G"
};



const int action_sequences[][MAX_SEQUENCE_LENGTH] = {
  {0, 1, 0}, // Palm -> Fist -> Palm
  {1, 0, 1}, // Fist -> Palm -> Fist
};

const String action_sequence_names[] = {
  "Palm to Fist to Palm",
  "Fist to Palm to Fist"
};

uint8_t history[HIST_LENGTH] = {0};
int index = 1;
String curAction;



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


    for (int j = 0; j < 2; j++) {
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
      espSerial.println(gesture_names[i]);
      return;
    }
  }
  Serial.println("No gesture detected");
}



void detectActionSequence() {
  for (int a = 0; a < sizeof(action_sequences) / sizeof(action_sequences[0]); a++) {
    int len = sizeof(action_sequences[a]) / sizeof(action_sequences[a][0]);
    bool match = true;

    for (int i = 0; i < len; i++) {
      int hist_index = (index - i - 1 + HIST_LENGTH) % HIST_LENGTH;
      if (history[hist_index] != action_sequences[a][len - i - 1]) {
        match = false;
        break;
      }
    }

    if (match) {
      Serial.println("Detected Action Sequence: " + action_sequence_names[a]);
      return;
    }
  }
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
  detectActionSequence();
  delay(1000);
}

