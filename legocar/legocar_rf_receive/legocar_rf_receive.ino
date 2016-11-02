#include <Servo.h>

Servo myServo;

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

const int servoPin = 9;
const int hornPin = 5;

const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 6;

String bufStr;
String hornStr;
String xStr;
String yStr;

int xVal;
int yVal;
int angle;
int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 0;
int hornVal;

unsigned long previousSteeringMillis = 0;
// will store last time LED was updated
// constants won't change :
const long steeringInterval = 100;  

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  
  if (!driver.init())
    //Serial.println("init failed");
  
  pinMode(controlPin1,OUTPUT);
  pinMode(controlPin2,OUTPUT);
  pinMode(enablePin,OUTPUT);
  digitalWrite(enablePin,LOW);
  
  pinMode(hornPin,OUTPUT);
  
  myServo.attach(servoPin);
}

void loop() {

    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    unsigned long currentMillis = millis();

    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        // Message with a good checksum received, print it.
        //Serial.print("Got: ");
        //Serial.println((char*)buf);

        bufStr = String((char*)buf);

        int pipeIndex = bufStr.indexOf('|');
        int secondPipeIndex = bufStr.indexOf('|', pipeIndex+1);

        hornStr = bufStr.substring(0, pipeIndex);
        xStr = bufStr.substring(pipeIndex+1, secondPipeIndex);
        yStr = bufStr.substring(secondPipeIndex+1);
//        Serial.println(hornStr);
//        Serial.println(xStr);
//        Serial.println(yStr);
//        Serial.println();
        hornVal = hornStr != "1";
        xVal = xStr.toInt();
        yVal = yStr.toInt();
        
    }

  
  if (currentMillis - previousSteeringMillis >= steeringInterval) {
    // save the last time you blinked the LED
    previousSteeringMillis = currentMillis;
//    xVal = analogRead(xPin);
//    yVal = analogRead(yPin);
//    hornVal = digitalRead(hornPin);
//    if(hornActive == 0 && hornVal == 0){
//      hornActive = 1;
//      previousHornMillis = currentMillis;
//    }
    
    Serial.print("X: ");
    Serial.print(xVal,DEC);
    Serial.print("|Y: ");
    Serial.print(yVal,DEC);
    Serial.print("|Z: ");
    Serial.print(hornVal);
    
    angle = map(xVal, 0, 1023, 20, 159);
    myServo.write(angle);
    
    Serial.print("|Angle: ");
    Serial.println(angle);
  
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

  /*if(angle > 178){
  digitalWrite(debug1,HIGH);
  }
  if(angle < 1){
  digitalWrite(debug2,HIGH);
  }*/

  digitalWrite(hornPin,hornVal);
                   
}
