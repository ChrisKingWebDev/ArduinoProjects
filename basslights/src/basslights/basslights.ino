#include <Adafruit_NeoPixel.h>

#define PIN 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(95, PIN, NEO_GRB + NEO_KHZ800);

// These constants won't change.  They're used to give names
// to the pins used:
//const int analogInPin = 1;  // Analog input pin that the potentiometer is attached to
//const int analogOutPin = 9; // Analog output pin that the LED is attached to
//const int digitalOutPin = 8;
//
//int sensorValue = 0;        // value read from the pot
//int outputValue = 0;        // value output to the PWM (analog out)
bool lightsOn = false;
int blue;
int red;
int pwmLevels [16] = {1,2,3,4,6,8,12,16,23,32,45,64,90,128,280,255};

void setup() {
  // initialize serial communications at 9600 bps:
  // Serial.begin(9600);
  //pinMode(digitalOutPin,OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // read the analog in value:
//  sensorValue = analogRead(analogInPin);
//  // map it to the range of the analog out:
//  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
// analogWrite(analogOutPin, outputValue);
 // if(outputValue > 40) {
 //   digitalWrite(digitalOutPin, HIGH);
 // } else {
 //   digitalWrite(digitalOutPin, LOW);
 // }
// Serial.println(sensorValue);
  //if(outputValue > 40 && lightsOn == false) {
    // Serial.println("In lights on");
    lightsOn = true;
//    for(int j = 1;  j <= 20; j++) {
//      int blue = linearPWM (255, 20, j);
//      int red = linearPWM (50, 20, j);
//      colorSet(strip.Color(red, 0, blue)); // Blue
//      delay(1);
//    }
//    delay(25);
//    for(int j = 20;  j >= 0; j--) {
//      int blue = linearPWM (255, 20, j);
//      int red = linearPWM (50, 20, j);
//      colorSet(strip.Color(red, 0, blue)); // Blue
//      delay(20);
//    }
//    colorSet(strip.Color(0, 0, 0)); // Blue
    for(int j = 0; j <= (strip.numPixels() + 32); j++) {
      colorSet(strip.Color(0, 0, 0));
      for(int i = 0; i <= 32; i++) {
        int currentPixel = j - i;
        if (currentPixel > 0){
          if(i < 17) {
            blue = pwmLevels[i];
            red = pwmLevels[i];
          } else {
            blue = pwmLevels[16 - (i - 16)];
            red = pwmLevels[16 - (i - 16)];
          }
//          if(j == 151){
//            Serial.print(currentPixel);
//            Serial.print(" | " );
//            Serial.print(red);
//            Serial.print(" | " );
//            Serial.println(blue);
//          }
          strip.setPixelColor(currentPixel, strip.Color(red, 0, blue));
        }
      }
      strip.setPixelColor(j - 32, strip.Color(0, 0, 0));
      strip.show();
    }
    //colorSet(strip.Color(0, 0, 0)); // Blue
//    for(int j = 0; j < 120; j++) {
//      strip.setPixelColor(j, strip.Color(40, 0, 255));
//      strip.setPixelColor(j-1, strip.Color(0, 0, 50));
//      strip.show();
//    }
    lightsOn = false;
  //}


  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);
}

void colorSet(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
}
