//DECLARATION - FIELDS =======================================================================

//For the solenoids and 5-button keypad : 
const int btnPin = A0; //connection with the buttons matrix (analog signal)
const int sole1Pin = 2; //connection with the first solenoide (digital signal)
const int sole2Pin = 3;
const int sole3Pin = 4;
const int sole4Pin = 5;
const int sole5Pin = 6;
int n = -1; //integer initialize in order to link interval analogRead value of matrix buttons to one solenoid

//For the motor DC and the potentiometer : 
const int motor = 9; //(pwm signal)
int Potentiometer = A1; //(analog signal)
const int directionA = 12; //first direction of the motor 
const int directionB = 13;
int ValPo = 0;
int Pwm;

//For the ignition of the system (remote sensor + 3LEDs) :
//PIN 7 is utilized for the remote sensor (trigger + echo)
const int Red = 11;
const int Orange = 10;
const int Blue = 8;
int count ;
int cm = 0;

//MAIN ======================================================================================

void setup() {
  Serial.begin(9600); //initialize serial USB
  pinMode(btnPin, INPUT); //initialize the matrix bouton
  pinMode(sole1Pin, OUTPUT); //initialize the connection 
  pinMode(sole2Pin, OUTPUT);
  pinMode(sole3Pin, OUTPUT);
  pinMode(sole4Pin, OUTPUT);
  pinMode(sole5Pin, OUTPUT);
  digitalWrite(sole1Pin,LOW);
  digitalWrite(sole2Pin,LOW);
  digitalWrite(sole3Pin,LOW);
  digitalWrite(sole4Pin,LOW);
  digitalWrite(sole5Pin,LOW);
  
  pinMode(motor, OUTPUT); //initialize the speed pin of the motor

  pinMode(Red, OUTPUT); //initialize LED pin
  pinMode(Orange,OUTPUT);
  pinMode(Blue,OUTPUT);
}
void loop() {
  digitalWrite(Orange,LOW);
  digitalWrite(Blue,LOW);
  digitalWrite(Red,HIGH);
  cm = readUltrasonicDistance(7,7) * 0.01723; //to have integer value in centimeter
  if(cm>0 && cm<15){ //verify the proximity to actionate system
    count = 0;
    while(count != 100){ //start stopwatch
      ValueBtnMatrix();
      Enslavement();
      if(count<50){
        digitalWrite(Blue, HIGH);
        digitalWrite(Orange,LOW);
        digitalWrite(Red,LOW);
      }
      else{
        digitalWrite(Orange,HIGH);
        digitalWrite(Blue,LOW);
        digitalWrite(Red,LOW);
      }
      count ++;
      delay(100);
    }
  }
  delay(100);
  //HelpMain();
}

//FUNCTIONS =================================================================================

/* For the solenoids and the buttons matrix :
 * I used : 
 ** - 5 Solenoids Electro-magnet Push Pull - 12V
 ** - A 5-button keypad Octopus EF04017
 */
void ValueBtnMatrix() //on Vcc = 3.3V !!!
{
  n = analogRead(btnPin); //read the value of the buttons matrix
  if(n>=0 && n<25){ //A==0
    HorLSolenoide(sole1Pin);
  }
  else if(25<=n && n<50){ //B==30
    HorLSolenoide(sole2Pin);
  }
  else if(50<=n && n<75){ //C==60
    HorLSolenoide(sole3Pin);
  }
  else if(75<=n && n<100){ //D==(87||88)
    HorLSolenoide(sole4Pin);
  }
  else if(100<=n && n<600){ //E==(367)
    HorLSolenoide(sole5Pin);
  }
  delay(100);
}
void HorLSolenoide(int solenPin)
{
  if(digitalRead(solenPin) == LOW){ //verify the previous state 
    digitalWrite(solenPin,HIGH);
  }
  else {
    digitalWrite(solenPin,LOW);
  }
}

//For the remote sensor (I used an SDM-IO - only 3pins !):
long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin,LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

//For the motor DC (I used Metal DC geared Motor-Encoder - 6V - 300RPM):
void Enslavement(){
  ValPo = analogRead(Potentiometer)*0.2492668622;
  analogWrite(motor, ValPo);
  digitalWrite(directionA,HIGH);
  digitalWrite(directionB,LOW);
  /*
  Pwm = map(ValPo, 0, 1020, 0, 255);
  if(Pwm>0){
    analogWrite(motor, Pwm);
  }
  */
}

//IF REMOTE SENSOR IS BUGGING (because of bad connection at last minute) :
void HelpMain(){
  ValueBtnMatrix();
  Enslavement();
}
