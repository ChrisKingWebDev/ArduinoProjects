#include <Servo.h>
Servo myServo;

const int xPin = A0;
const int yPin = A1;
const int hornPin = 4;
const int servoPin = 9;
const int buzzerPin = 5;

const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 6;

//const int debug1 = 7;
//const int debug2 = 8;

int xVal;
int yVal;
int hornVal;
int angle;
int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 0;

int notes[] = {440,440,440,588,740};
int noteLength[] = {150,150,150,400,300};

unsigned long previousSteeringMillis = 0;
unsigned long previousHornMillis = 0; 
// will store last time LED was updated
// constants won't change :
const long steeringInterval = 100;  
const long hornPause = 60;

bool hornActive = 0;
int currentNote = 0;

void setup()
{
  pinMode(controlPin1,OUTPUT);
  pinMode(controlPin2,OUTPUT);
  pinMode(enablePin,OUTPUT);
  digitalWrite(enablePin,LOW);
  
  pinMode(hornPin,INPUT);
  digitalWrite(hornPin, HIGH);
  pinMode(buzzerPin,OUTPUT);
  
  //digitalWrite(debug1, HIGH);
  //digitalWrite(debug2, HIGH);
  myServo.attach(servoPin);
  //Serial.begin(9600);
}

void loop()
{
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousSteeringMillis >= steeringInterval) {
    // save the last time you blinked the LED
    previousSteeringMillis = currentMillis;
    xVal = analogRead(xPin);
    yVal = analogRead(yPin);
    hornVal = digitalRead(hornPin);
    if(hornActive == 0 && hornVal == 0){
      hornActive = 1;
      previousHornMillis = currentMillis;
    }
    
    /*Serial.print("X: ");
    Serial.print(xVal,DEC);
    Serial.print("|Y: ");
    Serial.print(yVal,DEC);
    Serial.print("|Z: ");
    Serial.print(hornVal);*/
    
    angle = map(xVal, 0, 1023, 0, 179);
    myServo.write(angle);
    
    //Serial.print("|Angle: ");
    //Serial.println(angle);
  
    motorEnabled = 0;
    if(yVal > 600) {
      motorEnabled = 1;
      motorDirection = 1;
      motorSpeed = 255; //map(yVal,600,1023,0,255);
    } else if (yVal < 400) {
      motorEnabled = 1;
      motorDirection = 0;
      motorSpeed = 255; //map(yVal,400,0,0,255);
    }
    //motorSpeed = 50;
  
    if(motorDirection == 1){
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
    } else {
      digitalWrite(controlPin1, LOW);
      digitalWrite(controlPin2, HIGH);
    }
    if(motorEnabled == 1){
      analogWrite(enablePin, motorSpeed);
    } else {
      analogWrite(enablePin, 0);
    }

    //Serial.println("");
  }
  
  //digitalWrite(debug1,LOW);
  //digitalWrite(debug2,LOW);

  /*if(angle > 178){
  digitalWrite(debug1,HIGH);
  }
  if(angle < 1){
  digitalWrite(debug2,HIGH);
  }*/

  if (hornActive == 1){
    if (currentMillis - previousHornMillis < noteLength[currentNote]) {
      //digitalWrite(debug1,HIGH);
      tone(buzzerPin, notes[currentNote]);
    }
    else if (currentMillis - previousHornMillis < (noteLength[currentNote] + hornPause)) {
      noTone(buzzerPin);
      //digitalWrite(debug2,HIGH);
    }
    else if (currentMillis - previousHornMillis >= (noteLength[currentNote] + hornPause)) {
      currentNote++;
      previousHornMillis = currentMillis;
      noTone(buzzerPin);
      if(currentNote == 5){
        currentNote = 0;
        hornActive = 0;
        
      //digitalWrite(debug1,HIGH);
      //digitalWrite(debug2,HIGH);
      }
    }
  }

  
  
  //delay(50);
}
