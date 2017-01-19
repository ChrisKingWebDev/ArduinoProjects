#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

int upPIN = 5;
int downPIN = 4;
int leftPIN  = 7;
int rightPIN = 6;

int upButtonState = 0;
int downButtonState = 0;
int leftButtonState = 0;
int rightButtonState = 0;

int prevUpButtonState = 0;
int prevDownButtonState = 0;
int prevLeftButtonState = 0;
int prevRightButtonState = 0;

int pX = 6;
int pY = 4;

int maxX = 15;
int maxY = 7;

void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("16x8 LED Mini Matrix Test");

  pinMode(upPIN, INPUT);
  pinMode(downPIN, INPUT);
  pinMode(leftPIN, INPUT);
  pinMode(rightPIN, INPUT);

  matrix.begin(0x70);  // pass in the address

  matrix.clear();      // clear display
  matrix.writeDisplay();
  // testLights();
}
void loop() {

  upButtonState = digitalRead(upPIN);
  if (upButtonState == HIGH && prevUpButtonState == LOW && pX < maxX ) {
    Serial.println("Up Button Press");
    pX++;
  }
  prevUpButtonState = upButtonState;

  downButtonState = digitalRead(downPIN);
  if (downButtonState == HIGH && prevDownButtonState == LOW && pX > 0 ) {
    Serial.println("Down Button Press");
    pX--;
  }
  prevDownButtonState = downButtonState;

  leftButtonState = digitalRead(leftPIN);
  if (leftButtonState == HIGH && prevLeftButtonState == LOW && pY < maxY ) {
    Serial.println("Left Button Press");
    pY++;
  }
  prevLeftButtonState = leftButtonState;

  rightButtonState = digitalRead(rightPIN);
  if (rightButtonState == HIGH && prevRightButtonState == LOW && pY > 0 ) {
    Serial.println("Right Button Press");
    pY--;
  }
  prevRightButtonState = rightButtonState;

  // Serial.print(String(digitalRead(upPIN)) + " " + upButtonState + " | ");
  // Serial.print(String(digitalRead(downPIN)) + " " + downButtonState + " | ");
  // Serial.print(String(prevLeftButtonState) + " " + leftButtonState + " | ");
  // Serial.print(String(prevRightButtonState) + " " + rightButtonState + " | ");
  // Serial.println(String(pX) + " " + pY);

  matrix.clear();
  matrix.drawPixel(pY, pX, LED_ON);
  matrix.writeDisplay();
  delay(50);

}

void testLights() {

  delay(100);

  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 16; x++) {
      matrix.drawPixel(y, x, LED_ON);
      matrix.writeDisplay();
      delay(5);
    }
  }
  matrix.clear();      // clear display
  matrix.writeDisplay();
  delay(100);
  for(int x = 0; x < 16; x++) {
    for(int y = 0; y < 8; y++) {
      matrix.drawPixel(y, x, LED_ON);
      matrix.writeDisplay();
      delay(5);
    }
  }
  matrix.clear();      // clear display
  matrix.writeDisplay();
  delay(100);
  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 16; x++) {
      matrix.drawPixel(y, x, LED_ON);
    }
    matrix.writeDisplay();
    delay(50);
  }
  matrix.clear();      // clear display
  matrix.writeDisplay();
  delay(100);
  for(int x = 0; x < 16; x++) {
    for(int y = 0; y < 8; y++) {
      matrix.drawPixel(y, x, LED_ON);
    }
    matrix.writeDisplay();
    delay(50);
  }
  matrix.clear();      // clear display
  matrix.writeDisplay();
}
