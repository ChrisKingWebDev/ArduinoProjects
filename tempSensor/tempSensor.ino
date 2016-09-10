int analogPin = 0; //the thermistor attach to 
int beta = 4090; //the beta of the thermistor
int resistance = 10; //the value of the pull-down resistor

int yellowPin = 5;    // select the pin for the yellow LED
int redPin = 3;    // select the pin for the red LED


void setup()
{
}

void loop()
{
  //read thermistor value 
  long a =1023 - analogRead(analogPin);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  if(tempC < 24){
    tempC = 24;
  }
  if(tempC > 35){
    tempC = 35;
  }
  
  int redVal = map(tempC, 24, 35, 0, 255);
  int yellowVal = 255 - redVal;
  
  analogWrite(redPin, redVal);
  analogWrite(yellowPin, yellowVal);
  
  delay(20); //wait for 100 milliseconds
}
