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
int hoursDrip = -1;

int secondsSolid = N_LEDS + 1;
int minutesSolid = N_LEDS + 1;
int hoursSolid = N_LEDS + 1;

void setup () {
  currentSecond = 0;
  currentMinute = 0;
  currentHour = 0;
  
  rtc.begin();

  if (! rtc.isrunning()) {
    //Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    //Serial.println("RTC is ALREADY running!");
  }

  strip.begin();
}

void loop () {
    DateTime now = rtc.now();

    //check for a new second
    currentSecond = now.second();
    if (currentSecond != prevSecond) {
      prevSecond = currentSecond;

      //set up a new drip if it's not 0 seconds
      if  (currentSecond > 0)  {
        secondsDrip = 0;
      }
    }

    currentMinute = now.minute();
    if (currentMinute != prevMinute) {
      prevMinute = currentMinute;

      //set up a new drip if it's not 0 minutes
      if (currentMinute > 0) {
        minutesDrip = 0;
      }
    }

    currentHour = now.hour();
    if (currentHour != prevHour) {
      prevHour = currentHour;

      //set up a new drip if it's not 0 hours
      if (currentHour > 0) {
        hoursDrip = 0;
      }
      hoursSolid = N_LEDS - currentHour + 1;
    }

    secondsSolid = N_LEDS - currentSecond + 1;
    minutesSolid = N_LEDS - currentMinute + 1;
    hoursSolid = N_LEDS - currentHour + 1;

    for(int k=0; k<strip.numPixels(); k++) {
      int red = 0;
      int blue = 0;
      int green = 0;

      if (k == secondsDrip || k >= secondsSolid || (secondsDrip == -1 && (k == secondsSolid - 1))) {
        red = 100;
      }

      if (k == minutesDrip || k >= minutesSolid || (minutesDrip == -1 && (k == minutesSolid - 1))) {
        if (red == 0) {
          blue = 100;
        } else {
          red = 50;
          blue = 50;  
        }
      }

      if (k == hoursDrip || k >= hoursSolid || (hoursDrip == -1 && (k == hoursSolid -1 ))) {
        if (red == 0 && blue == 0) {
          green = 100;
        } else if (red > 0 && blue == 0) {
          red = 50;
          green = 50; 
        } else if (red == 0 && blue > 0) {
          blue = 50;
          green = 50; 
        } else if (red > 0 && blue > 0) {
          blue = 0;
          red = 80;
          green = 20; 
        }
      } 
      strip.setPixelColor(k, strip.Color(red, green, blue));
    }
    
    strip.show();

    if (secondsDrip != -1 && secondsDrip < (secondsSolid - 1)) {
      secondsDrip++;
    }
    if (secondsDrip == (secondsSolid - 1)) {
      secondsDrip = -1;
    }

    if (minutesDrip != -1 && minutesDrip < (minutesSolid - 1)) {
      minutesDrip++;
    }
    if (minutesDrip == (minutesSolid - 1)) {
      minutesDrip = -1;
    }

    if (hoursDrip != -1 && hoursDrip < (hoursSolid -1)) {
      hoursDrip++;
    }
    if (hoursDrip == (hoursSolid - 1)) {
      hoursDrip = -1;
    }
      
      
    delay(10);
}
