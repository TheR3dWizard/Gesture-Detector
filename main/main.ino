#include <BasicLinearAlgebra.h>
#include <ElementStorage.h>

using namespace std;
using namespace BLA;

const int threshold = 250;
int flexPins[5] = {A0,A1,A2,A3,A4};
Gesture signs[2];

class Gesture{
  public:
    String value;
    Matrix<5> state;
    
    boolean isGesture(Matrix<5> input){
      Matrix<5> difference = state - input; //find difference matrix
      int cost = 0;
      for(int i = 0;i<5;i++){
        cost += pow(difference(i,0),2);
      }
      cost = abs(cost);
      Serial.print("Cost is: ");
      Serial.println(cost);
      return cost < threshold;
    }

    Gesture(String val, Matrix<5> flexData){
      value = val;
      state = flexData;
    }

    Gesture() : value(""), state() {}

};



int readFlex(int index){
  int val = analogRead(flexPins[index]);
  int angle = map(val,0,1023,0,180);
  return angle;
}

Matrix<5> readGesture(){ 
  Matrix<5> gesture;
  gesture(0, 0) = readFlex(0);
  gesture(1, 0) = readFlex(1);
  gesture(2, 0) = readFlex(2);
  gesture(3, 0) = readFlex(3);
  gesture(4, 0) = readFlex(4);
  return gesture;
}

void initArray(){
  //thumb index middle ring pinky
  signs[0] = Gesture("A",{180,180,180,180,180});
  signs[1] = Gesture("B",{180,0,0,0,0});
}

void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  Serial.begin(9600);
  initArray();
  driver();
}

void loop() {

}

void driver(){
  
  Matrix<5> sign = {180,3,0,0,0};
   for(int i = 0;i<2;i++){
      if(signs[i].isGesture(sign)){
        Serial.print("The sign is ");
        Serial.println(signs[i].value);
      }
    }
}
