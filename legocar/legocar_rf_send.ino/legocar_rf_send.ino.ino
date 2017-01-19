#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

const int xPin = A0;
const int yPin = A1;
const int hornPin = 4;
int xVal;
int yVal;
int hornVal;
String sendStr  = "";

void setup() {
  
  
//  Serial.begin(9600);
  if (!driver.init())
     //Serial.println("init failed");
 
  // Data
  //pinMode(8, OUTPUT);  // Either way, we'll use pin 7 to drive the data pin of the transmitter.
  
  
  pinMode(hornPin,INPUT);
  digitalWrite(hornPin, HIGH);
  
}

void loop() {

      sendStr = "";
      
      xVal = analogRead(xPin);
      yVal = analogRead(yPin);
      hornVal = digitalRead(hornPin);

      //sendCode = hornVal ? 1000000 : 0000000;

      //sendCode = sendCode + (xVal * 1000);
      
      //sendCode = sendCode + yVal;
//
//      Serial.print(hornVal);
//      Serial.print("|");
//      Serial.print(xVal);
//      Serial.print("|");
//      Serial.println(yVal);

      sendStr = sendStr + String(hornVal);
      sendStr = sendStr + "|";
      sendStr = sendStr + String(xVal);
      sendStr = sendStr + "|";
      sendStr = sendStr + String(yVal);
      
//      Serial.println(sendStr);
      
      //mySwitch.send(sendCode, 24);

      char charBuf[sendStr.length()+1];
      sendStr.toCharArray(charBuf,sendStr.length()+1);
      //Serial.println(charBuf);
      //vw_send((uint8_t *)charBuf, (uint8_t)sendStr.length()+1); 
      //vw_wait_tx(); // Wait until the whole message is gone

      driver.send((uint8_t *)charBuf, sendStr.length()+1);
      driver.waitPacketSent();
    
      delay(5);
  
}
