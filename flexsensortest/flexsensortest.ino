const int led = 11;
const int flexpin = A0;
int val = 0;
int op = 0;
int prevop = 0;
const int threshold[3] = {60,120,240};


void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  val = analogRead(A0);
  // Serial.println(val);
  op = map(val,560,850,0,180);
  // op -= 145;
  if(op != prevop){
    // Serial.print(val); //raw value
    // Serial.print(",");
    Serial.print(op); //mapped value
    prevop = op;
    for(int i = 0;i < 3;i++){
      if(op < threshold[i]){
        Serial.print(",");
        Serial.print(i);
        break;
      }
    }
    Serial.println();
  }

  

  analogWrite(11,op);
  delay(100);
}
