const int Note_C  = 239;
const int Note_CS = 225;
const int Note_D  = 213;
const int Note_DS = 201;
const int Note_E  = 190;
const int Note_F  = 179;
const int Note_FS = 169;
const int Note_G  = 159;
const int Note_GS = 150;
const int Note_A  = 142;
const int Note_AS = 134;
const int Note_B  = 127;

int notes[] = {Note_C,Note_C,Note_C,Note_F,Note_A}; 
int noteLength[] = {150,150,150,400,300};
const long hornPause = 60;

unsigned long previousHornMillis = 0; 
bool hornActive = 0;
int currentNote = 0;

int hornVal;
int speakerPin = 1;
int hornInputPin = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(hornInputPin,INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  hornVal = digitalRead(hornInputPin);

  if(hornActive == 0 && hornVal == 1){
    hornActive = 1;
    previousHornMillis = currentMillis;
  }
  
  if (hornActive == 1){
    if (currentMillis - previousHornMillis < noteLength[currentNote]) {
      TinyTone(notes[currentNote], 4);
    }
    else if (currentMillis - previousHornMillis < (noteLength[currentNote] + hornPause)) {
      StopTone();
    }
    else if (currentMillis - previousHornMillis >= (noteLength[currentNote] + hornPause)) {
      currentNote++;
      previousHornMillis = currentMillis;
      if(currentNote == 5){
        currentNote = 0;
        hornActive = 0;
      }
    }
  }
}

void TinyTone(unsigned char divisor, unsigned char octave)
{
  TCCR1 = 0x90 | (8-octave); // for 1MHz clock
  //TCCR1 = 0x90 | (11-octave); // for 8MHz clock
  OCR1C = divisor-1;         // set the OCR
  //delay(duration);         // stop the counter
}

void StopTone(){
  TCCR1 = 0x90;
}
