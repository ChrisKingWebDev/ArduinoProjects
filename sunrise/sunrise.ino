#include <Adafruit_NeoPixel.h>

#define PIN      6
float timePerColor = (1.0 * 60.0 * 1000.0); //minutes in milliseconds
//float timePerColor = (50 * 25); //minutes in milliseconds
#define STEPS 50.0
#define N_LEDS 59

int mills_per_step;

float red = 0;
float green = 0;
float blue = 0;

float darkBlue[3] = {0, 0, 20};
float darkPurple[3] = {20, 0, 20};
float maroon[3] = {19, 7, 13};
float darkRed[3] = {50, 0, 0};
float orange[3] = {128, 51, 0};
float yellow[3] = {150, 100, 0};
float white[3] = {255, 255, 255};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  mills_per_step = timePerColor / STEPS;
}

void loop() {

  red = 0;
  green = 0;
  blue = 0;

  setStripColor();

  fadeToColor(darkBlue);
  fadeToColor(darkPurple);
  fadeToColor(maroon);
  fadeToColor(darkRed);
  fadeToColor(orange);
  fadeToColor(yellow);
  fadeToColor(white);


  delay(1000);

  for(int k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor(k, strip.Color(0, 0, 0));
  }
  strip.show();
  delay(1000);
}

void setStripColor() {
  for(int k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor(k, strip.Color(red, green, blue));
  }
  strip.show();
}

void fadeToColor(float color[3]) {
  float nRed = color[0];
  float nGreen = color[1];
  float nBlue = color[2];
  for (int j = 0; j <= STEPS; j++){

    int rDiff = nRed - red;
    red = red + ((rDiff / STEPS) * j);

    int gDiff = nGreen - green;
    green = green + ((gDiff / STEPS) * j);

    int bDiff = nBlue - blue;
    blue = blue + ((bDiff / STEPS) * j);

    setStripColor();
    delay(mills_per_step);

  }
}
