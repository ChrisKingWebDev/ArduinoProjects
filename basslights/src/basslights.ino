// #include <Adafruit_NeoPixel.h>
//
// #define PIN 3
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);
//
// // These constants won't change.  They're used to give names
// // to the pins used:
// const int analogInPin = 1;  // Analog input pin that the potentiometer is attached to
// //const int analogOutPin = 9; // Analog output pin that the LED is attached to
// //const int digitalOutPin = 8;
// //
// int sensorValue = 0;        // value read from the pot
// int outputValue = 0;        // value output to the PWM (analog out)
// bool lightsOn = false;
// int blue;
// int red;
// int pwmLevels [16] = {1,2,3,4,6,8,12,16,23,32,45,64,90,128,280,255};
//
// void setup() {
//   // initialize serial communications at 9600 bps:
//    Serial.begin(9600);
//   //pinMode(digitalOutPin,OUTPUT);
//
//   strip.begin();
//   strip.show(); // Initialize all pixels to 'off'
// }
//
// void loop() {
//   // read the analog in value:
//  sensorValue = analogRead(analogInPin);
//  // map it to the range of the analog out:
//  outputValue = map(sensorValue, 0, 1023, 0, 255);
//   // change the analog out value:
// // analogWrite(analogOutPin, outputValue);
//  // if(outputValue > 40) {
//  //   digitalWrite(digitalOutPin, HIGH);
//  // } else {
//  //   digitalWrite(digitalOutPin, LOW);
//  // }
//  Serial.println(sensorValue);
//   if(sensorValue > 40 && lightsOn == false) {
//     // Serial.println("In lights on");
//     lightsOn = true;
// //    for(int j = 1;  j <= 20; j++) {
// //      int blue = linearPWM (255, 20, j);
// //      int red = linearPWM (50, 20, j);
// //      colorSet(strip.Color(red, 0, blue)); // Blue
// //      delay(1);
// //    }
// //    delay(25);
// //    for(int j = 20;  j >= 0; j--) {
// //      int blue = linearPWM (255, 20, j);
// //      int red = linearPWM (50, 20, j);
// //      colorSet(strip.Color(red, 0, blue)); // Blue
// //      delay(20);
// //    }
// //    colorSet(strip.Color(0, 0, 0)); // Blue
//     for(int j = 0; j <= (strip.numPixels() + 32); j++) {
//       colorSet(strip.Color(0, 0, 0));
//       for(int i = 0; i <= 32; i++) {
//         int currentPixel = j - i;
//         if (currentPixel > 0){
//           if(i < 17) {
//             blue = pwmLevels[i];
//             red = pwmLevels[i];
//           } else {
//             blue = pwmLevels[16 - (i - 16)];
//             red = pwmLevels[16 - (i - 16)];
//           }
// //          if(j == 151){
// //            Serial.print(currentPixel);
// //            Serial.print(" | " );
// //            Serial.print(red);
// //            Serial.print(" | " );
// //            Serial.println(blue);
// //          }
//           strip.setPixelColor(currentPixel, strip.Color(red, 0, blue));
//         }
//       }
//       strip.setPixelColor(j - 32, strip.Color(0, 0, 0));
//       strip.show();
//     }
//     //colorSet(strip.Color(0, 0, 0)); // Blue
// //    for(int j = 0; j < 120; j++) {
// //      strip.setPixelColor(j, strip.Color(40, 0, 255));
// //      strip.setPixelColor(j-1, strip.Color(0, 0, 50));
// //      strip.show();
// //    }
//     lightsOn = false;
//   }
//
//
//   // wait 2 milliseconds before the next loop
//   // for the analog-to-digital converter to settle
//   // after the last reading:
//   delay(2);
// }
//
// void colorSet(uint32_t c) {
//   for(uint16_t i=0; i<strip.numPixels(); i++) {
//     strip.setPixelColor(i, c);
//   }
// }

// /**
//  * Drum lights: Trigger-driven drum illumination
//  * Alec Smecher, 2014
//  * http://www.cassettepunk.com
//  */
//
// #include <Adafruit_NeoPixel.h>
//
// /**
//  * Pin configuration
//  */
// #define STRIP_PIN 8 // The digital pin # for the neopixel output
// #define PIEZO_DIG_PIN 4 // The digital pin # for the piezo input
// #define PIEZO_ANALOG_PIN A0 // The analog pin # for the piezo input
//
// /**
//  * Neopixel strip lengths for each unit
//  */
// #define STRIP_LENGTH 120
//
//
// int sensorValue = 0;        // value read from the pot
// double outputValue = 0;
//
// uint32_t Wheel(byte WheelPos, float intensity = 1);
//
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);
//
// void setup() {
//   Serial.begin(9600);
//   pinMode(PIEZO_DIG_PIN, INPUT);
//   strip.begin();
// }
//
// unsigned char n=0; // Counter for colour wheel effect
//
// void loop() {
//   // Perform the pixel chase
//   for (int i=strip.numPixels(); i>0; i--) {
//     strip.setPixelColor(i, strip.getPixelColor(i-1));
//   }
//
//   // Set the first pixel using the current piezo value
//   sensorValue = analogRead(PIEZO_ANALOG_PIN);
//   outputValue = sensorValue/1024;
//   Serial.print("|| " + String(sensorValue) + " | ");
//   Serial.print(outputValue);
//   strip.setPixelColor(0, Wheel(n++, sensorValue/1024));
//   // strip.setPixelColor(0, Wheel(n++, 5));
//   // strip.setPixelColor(1, Wheel(n++, 100));
//   // strip.setPixelColor(2, Wheel(n++, 255));
//   // strip.setPixelColor(0, strip.Color(12, 0, 0));
//   // strip.setPixelColor(1, strip.Color(9, 0, 3));
//   // strip.setPixelColor(2, strip.Color(6, 0, 6));
//   // strip.setPixelColor(3, strip.Color(3, 0, 9));
//   // strip.setPixelColor(4, strip.Color(0, 0, 12));
//   // strip.setPixelColor(5, strip.Color(3, 0, 9));
//   // strip.setPixelColor(6, strip.Color(6, 0, 6));
//   // strip.setPixelColor(7, strip.Color(9, 0, 3));
//   // strip.setPixelColor(8, strip.Color(12, 0, 0));
//   //
//   // for (int i=0; i<9; i++) {
//   //   n = 255 / 8 * i;
//   //   strip.setPixelColor(10 + i, Wheel(n, 12));
//   // }
//   //
//   // strip.setPixelColor(20, strip.Color(100, 0, 0));
//   // strip.setPixelColor(21, strip.Color(75, 0, 25));
//   // strip.setPixelColor(22, strip.Color(50, 0, 50));
//   // strip.setPixelColor(23, strip.Color(25, 0, 75));
//   // strip.setPixelColor(24, strip.Color(0, 0, 100));
//   // strip.setPixelColor(25, strip.Color(25, 0, 75));
//   // strip.setPixelColor(26, strip.Color(50, 0, 50));
//   // strip.setPixelColor(27, strip.Color(75, 0, 25));
//   // strip.setPixelColor(28, strip.Color(100, 0, 0));
//   //
//   // for (int i=0; i<9; i++) {
//   //   n = 255 / 8 * i;
//   //   strip.setPixelColor(30 + i, Wheel(n, 100));
//   // }
//   //
//   // strip.setPixelColor(40, strip.Color(255, 0, 0));
//   // strip.setPixelColor(41, strip.Color(191, 0, 64));
//   // strip.setPixelColor(42, strip.Color(127, 0, 127));
//   // strip.setPixelColor(43, strip.Color(64, 0, 191));
//   // strip.setPixelColor(44, strip.Color(0, 0, 255));
//   // strip.setPixelColor(45, strip.Color(64, 0, 191));
//   // strip.setPixelColor(46, strip.Color(127, 0, 127));
//   // strip.setPixelColor(47, strip.Color(191, 0, 64));
//   // strip.setPixelColor(48, strip.Color(255, 0, 0));
//   //
//   // for (int i=0; i<9; i++) {
//   //   n = 255 / 8 * i;
//   //   strip.setPixelColor(50 + i, Wheel(n, 100));
//   // }
//
//   // Update the strip
//   strip.show();
//   delay(50);
// }
//
// // Input a value 0 to 255 to get a color value.
// // The colours are a transition r - g - b - back to r.
// // This function adapted from Adafruit's demo code to add intensity.
// uint32_t Wheel(byte WheelPos, float intensity) {
//
//   Serial.print(" || " + String(WheelPos) + " | ");
//   Serial.println(intensity);
//   return strip.Color(intensity * (abs(WheelPos - 127.5) / 127.5), 0, intensity * (1 - (abs(WheelPos - 127.5) / 127.5)));
//
// }

/**
 * Drum lights: Trigger-driven drum illumination
 * Alec Smecher, 2014
 * http://www.cassettepunk.com
 */

#include <Adafruit_NeoPixel.h>

/**
 * Pin configuration
 */
#define STRIP_PIN 4 // The digital pin # for the neopixel output
#define PIEZO_ANALOG_PIN A1 // The analog pin # for the piezo input

#define STRIP_LENGTH 120

int sensorValue = 0;        // value read from the pot
double outputValue = 0;
int maxSensor = 0;

uint32_t Wheel(byte WheelPos, float intensity = 1);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Serial.begin(9600);
  strip.begin();


  strip.setPixelColor(8, strip.Color(30, 0, 0));
  strip.show();
  delay(200);
  strip.setPixelColor(9, strip.Color(15, 15, 0));
  strip.show();
  delay(200);
  strip.setPixelColor(10, strip.Color(0, 30, 0));
  strip.show();
  delay(200);
  strip.setPixelColor(8, strip.Color(0, 0, 0));
  strip.setPixelColor(9, strip.Color(0, 0, 0));
  strip.setPixelColor(10, strip.Color(0, 0, 0));
  strip.show();
}

unsigned char n=0; // Counter for colour wheel effect

void loop() {
  sensorValue = analogRead(PIEZO_ANALOG_PIN);
  if(sensorValue < 5) {
    sensorValue = 0;
  }
  outputValue = (sensorValue) * 5;
  // if (sensorValue > 0) {
  //     Serial.print("|| " + String(sensorValue) + " | ");
  //     Serial.println(outputValue);
  // }

  // Perform the pixel chase
  for (int i=strip.numPixels(); i>0; i--) {
    strip.setPixelColor(i, strip.getPixelColor(i-1));
  }
  // for (int i=20; i>0; i--) {
  //   strip.setPixelColor(i, strip.getPixelColor(i-1));
  // }

  // Set the first pixel using the current piezo value
  if(millis() % 5 == 0) {
    n++;
  }
  strip.setPixelColor(0, Wheel(n, outputValue));

  // if(millis() % 500 == 0) {
  //   strip.setPixelColor(0, Wheel(n++, 100));
  // } else {
  //   strip.setPixelColor(0, Wheel(n++, 0));
  // }

  // for (int i=strip.numPixels(); i>=0; i--) {
  //   strip.setPixelColor(i, Wheel(n++, outputValue));
  // }

  // Update the strip
  strip.show();
  // delay(25);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// This function adapted from Adafruit's demo code to add intensity.
uint32_t Wheel(byte WheelPos, float intensity) {
  return strip.Color(intensity * (abs(WheelPos - 127.5) / 127.5), 0, intensity * (1 - (abs(WheelPos - 127.5) / 127.5)));
}
