
int currentLED = 2;
int sensorValue = 0;
int ledDir = 1;
int absSensor = 1;
int currentDelay = 25;

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //reset all the LEDS to low
  for(int i = 2; i < 8; i++){
    digitalWrite(i,LOW);
  }
  //set the current LED to high
  digitalWrite(currentLED,HIGH);

  //read the value from the resistor
  sensorValue = analogRead(A1);
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);

  //determine direction of the LED
  ledDir = (sensorValue > 512) ? 1 : -1;

  //set the current LED
  currentLED = currentLED + ledDir;
  
  if(currentLED == 8){
    currentLED=2;
  }  
  if(currentLED == 1){
    currentLED=7;
  }

  //get the value of the resistor away from 512 (the speed)
  absSensor = abs(sensorValue - 512);
  
  Serial.print(" absSensor: ");
  Serial.print(absSensor);

  //map the value to be between 10 and 400
  currentDelay = map(absSensor, 0,512,10,400);
  
  Serial.print(" currentDelay: ");
  Serial.println(currentDelay);

  //set a longer delay based on the higher value of the resistor
  delay(410 - currentDelay);
}
