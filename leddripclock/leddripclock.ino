// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

RTC_DS1307 rtc;
#define PIN      6
#define N_LEDS  59
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int currentSecond = 0;
int currentMinute = 0;
int currentHour = 0;
int prevSecond = 0;
int prevMinute = 0;
int prevHour = 0;

int secondsDrip = -1;
int minutesDrip = -1;
int hourDrip = -1;

int secondsSolid = N_LEDS + 1;
int minutesSolid = N_LEDS + 1;
int hoursSolid = N_LEDS + 1;

void setup () {
  currentSecond = 0;
  currentMinute = 0;
  currentHour = 0;

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("RTC is ALREADY running!");
  }

  strip.begin();
//  for(int k=0; k<strip.numPixels(); k++) {
//    strip.setPixelColor(k, strip.Color(0, 0, 0));
//  }
//  strip.show();

}

void loop () {
    DateTime now = rtc.now();

    //check for a new second
    currentSecond = now.second();
    if (currentSecond != prevSecond) {
      Serial.print("Current Second (s): ");
      Serial.println(currentSecond);
      Serial.print("Current Minute (s): ");
      Serial.println(currentMinute);
      prevSecond = currentSecond;

      //set up a new drip if it's not 0 seconds
      if  (currentSecond > 0)  {
        secondsDrip = 0;
      }
      secondsSolid = N_LEDS - currentSecond + 1;
    }

    currentMinute = now.minute();
    if (currentMinute != prevMinute) {
      prevMinute = currentMinute;

      //set up a new drip if it's not 0 minutes
      if (currentMinute > 0) {
        minutesDrip = 0;
      }
      minutesSolid = N_LEDS - currentMinute + 1;
      
      Serial.print("Current Minute: ");
      Serial.println(currentMinute);
      Serial.print("Minute Solid: ");
      Serial.println(minutesSolid);
    }

    for(int k=0; k<strip.numPixels(); k++) {
      int red = 0;
      int blue = 0;
      int green = 0;

      if (k == secondsDrip){
        red = 255;
      }
      if (k >= secondsSolid || (secondsDrip == -1 && (k == secondsSolid - 1))) {
        //red = ((k + 1) % 5 == 0) ? 255 : 20;
        red = 255;
      }

//      if (k == minutesDrip) {
//        blue = 255;  
//      }
//      if (k >= minutesSolid || (minutesDrip == -1 && (k == minutesSolid - 1))) {
//        //blue = ((k + 1) % 5 == 0) ? 255 : 20;
//        blue = 255;
//      }
        
      strip.setPixelColor(k, strip.Color(red, green, blue));
    }
    
    strip.show();

    if (secondsDrip != -1 && secondsDrip < (secondsSolid - 1)) {
      secondsDrip++;
    }
    if (secondsDrip == (secondsSolid - 1)) {
      secondsDrip = -1;
    }

//    if (minutesDrip != 1 && minutesDrip < (minutesSolid - 1)) {
//      minutesDrip++;
//    }
//    if (minutesDrip == (minutesSolid - 1)) {
//      minutesDrip = -1;
//    }
      
      
    delay(10);
}
