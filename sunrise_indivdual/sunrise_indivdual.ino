#include <Adafruit_NeoPixel.h>

#define PIN      6
long totalTime = (40 * 60 * 1000L);
#define STEPS 30.0
#define N_LEDS 59
long totalSteps;
long currentStep;
int colors = 7;

float ledInfo[N_LEDS][4];
int ledCount[N_LEDS];

uint16_t ledStep;
uint16_t colorStep;

double mills_per_step;

float darkBlue[3] = {0, 0, 20};
float darkPurple[3] = {20, 0, 20};
float maroon[3] = {19, 7, 13};
float darkRed[3] = {50, 0, 0};
float orange[3] = {128, 51, 0};
float yellow[3] = {150, 100, 0};
float white[3] = {255, 255, 255};
float currentColor[3];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();

  resetLeds();

  totalSteps = STEPS * N_LEDS * colors;
  mills_per_step = totalTime / totalSteps;

  currentStep = 0;
}

void loop() {

  resetStripColor();

  while(currentStep < totalSteps) {

    int i = -1;
    do {
      int randNumber = random(N_LEDS);
      if(ledCount[randNumber] < (STEPS * colors)){
        i = randNumber;
      }
    } while ( i == -1 );


      float cRed = ledInfo[i][0];
      float cGreen = ledInfo[i][1];
      float cBlue = ledInfo[i][2];

      ledCount[i]++;
      ledStep = ledCount[i];
      colorStep;

      if (ledStep < STEPS) {
        currentColor[0] = darkBlue[0];
        currentColor[1] = darkBlue[1];
        currentColor[2] = darkBlue[2];
        colorStep = ledStep;
      } else if (ledStep < (STEPS * 2)) {
        currentColor[0] = darkPurple[0];
        currentColor[1] = darkPurple[1];
        currentColor[2] = darkPurple[2];
        colorStep = ledStep - STEPS;
      } else if (ledStep < (STEPS * 3)) {
        currentColor[0] = maroon[0];
        currentColor[1] = maroon[1];
        currentColor[2] = maroon[2];
        colorStep = ledStep - (STEPS * 2);
      } else if (ledStep < (STEPS * 4)) {
        currentColor[0] = darkRed[0];
        currentColor[1] = darkRed[1];
        currentColor[2] = darkRed[2];
        colorStep = ledStep - (STEPS * 3);
      } else if (ledStep < (STEPS * 5)) {
        currentColor[0] = orange[0];
        currentColor[1] = orange[1];
        currentColor[2] = orange[2];
        colorStep = ledStep - (STEPS * 4);
      } else if (ledStep < (STEPS * 6)) {
        currentColor[0] = yellow[0];
        currentColor[1] = yellow[1];
        currentColor[2] = yellow[2];
        colorStep = ledStep - (STEPS * 5);
      } else if (ledStep < (STEPS * 7)) {
        currentColor[0] = white[0];
        currentColor[1] = white[1];
        currentColor[2] = white[2];
        colorStep = ledStep - (STEPS * 6);
      } else {
        currentColor[0] = 255;
        currentColor[1] = 255;
        currentColor[2] = 255;
      }

      float nRed = currentColor[0];
      float nGreen = currentColor[1];
      float nBlue = currentColor[2];


      int rDiff = nRed - cRed;
      cRed = cRed + (rDiff / (STEPS - colorStep));

      int gDiff = nGreen - cGreen;
      cGreen = cGreen + (gDiff / (STEPS - colorStep));

      int bDiff = nBlue - cBlue;
      cBlue = cBlue + (bDiff / (STEPS - colorStep));

      strip.setPixelColor(i, strip.Color(cRed, cGreen, cBlue));
      strip.show();

      currentStep++;

      ledInfo[i][0] = cRed;
      ledInfo[i][1] = cGreen;
      ledInfo[i][2] = cBlue;
      ledCount[i] = ledStep;

      delay(mills_per_step);
  }

  delay(5000);

  resetStripColor();

  delay(1000);

  currentStep = 0;
  resetLeds();

  totalSteps = STEPS * N_LEDS * colors;
  Serial.println(currentStep);
}

void resetLeds() {

  for(int i = 0; i <= N_LEDS; i++){
    ledInfo[i][0] = 0;
    ledInfo[i][1] = 0;
    ledInfo[i][2] = 0;
    ledCount[i] = 0;
  }
}

void resetStripColor() {
  for(int k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor(k, strip.Color(0, 0, 0));
  }
  strip.show();
}
