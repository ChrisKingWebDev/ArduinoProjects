// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

RTC_DS1307 rtc;
#define PIN      3
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

bool ascending = false;
bool didAscending = false;

int prevButton1State = 0;
int prevButton2State = 0;
int button1State = 0;
int button2State = 0;
int button1Pin = 1;
int button2Pin = 4;

void setup () {
  currentSecond = 0;
  currentMinute = 0;
  currentHour = 0;

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  
  //Serial.begin(57600);
  
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

  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);

  if (button1State == HIGH && button2State == HIGH && !didAscending) {
    ascending = !ascending;
    didAscending = true;
  }
  
  if (button1State == LOW && prevButton1State == HIGH && button2State == LOW && !didAscending) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute() + 1, now.second()));
    now = rtc.now();
    prevMinute = now.minute();
    minutesDrip = -1;
  }

  if (button2State == LOW && prevButton2State == HIGH && button1State == LOW && !didAscending) {
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute() - 1, now.second()));
    now = rtc.now();
    prevMinute = now.minute();
    minutesDrip = -1;
  }

  if(didAscending && button1State == LOW && button2State == LOW) {
    didAscending = false;
  }
  
  prevButton1State = button1State;
  prevButton2State = button2State;

  //check for a new second
  currentSecond = now.second();
    
    if (currentSecond != prevSecond) {
      prevSecond = currentSecond;

      //set up a new drip if it's not 0 seconds
      if  (currentSecond > 0)  {
        secondsDrip = N_LEDS;
      }
    }

    currentMinute = now.minute();
    if (currentMinute > prevMinute) {
      prevMinute = currentMinute;

      //set up a new drip if it's not 0 minutes
      if (currentMinute > 0) {
        minutesDrip = N_LEDS;
      }
    }

    currentHour = now.hour();
    if (currentHour != prevHour) {
      prevHour = currentHour;

      //set up a new drip if it's not 0 hours
      if (currentHour > 0) {
        hoursDrip = N_LEDS;
      }
    }

    secondsSolid = currentSecond - 1;
    minutesSolid = currentMinute - 1;
    hoursSolid = currentHour - 1;

    for(int k=0; k<strip.numPixels(); k++) {
      int red = 0;
      int blue = 0;
      int green = 0;

      if (
        (ascending && (
          k == (N_LEDS - secondsDrip)
          || k >= (N_LEDS - secondsSolid)
          || (secondsDrip == -1 && (k == (N_LEDS - secondsSolid - 1)))
        )) || //ascending matches
        (!ascending && (
          k == secondsDrip 
          || (k < secondsSolid) 
          || (secondsDrip == -1 && (k == secondsSolid))
          )) //descending matches
       ) {
        red = 100;
      }
      
      if (
        (ascending && (
          k == (N_LEDS - minutesDrip)
          || k >= (N_LEDS - minutesSolid)
          || (minutesDrip == -1 && (k == (N_LEDS - minutesSolid - 1)))
        )) || //ascending matches
        (!ascending && (
          k == minutesDrip 
          || (k < minutesSolid) 
          || (minutesDrip == -1 && (k == minutesSolid))
          )) //descending matches
       ) {
        if (red == 0) {
          blue = 100;
        } else {
          red = 50;
          blue = 50;  
        }
      }
      
      if (
        (ascending && (
          k == (N_LEDS - hoursDrip)
          || k >= (N_LEDS - hoursSolid)
          || (hoursDrip == -1 && (k == (N_LEDS - hoursSolid - 1)))
        )) || //ascending matches
        (!ascending && (
          k == hoursDrip 
          || (k < hoursSolid) 
          || (hoursDrip == -1 && (k == hoursSolid))
          )) //descending matches
       ) {
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

    if (secondsDrip != -1 && secondsDrip > secondsSolid) {
      secondsDrip--;
    }
    if (secondsDrip == secondsSolid) {
      secondsDrip = -1;
    }

    if (minutesDrip != -1 && minutesDrip > minutesSolid) {
      minutesDrip--;
    }
    if (minutesDrip == minutesSolid) {
      minutesDrip = -1;
    }

    if (hoursDrip != -1 && hoursDrip > hoursSolid) {
      hoursDrip--;
    }
    if (hoursDrip == hoursSolid) {
      hoursDrip = -1;
    }

    
      
      
    delay(10);
}
