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

int tonePin = 1;
int readPin = A2;

bool toneOn = false;

void setup() {
  pinMode(tonePin,OUTPUT);
}

void loop() {
  int keyVal = analogRead(readPin);
  if(keyVal == 1023){
    TinyTone(Note_A, 3);
  } else if(keyVal >= 990 && keyVal <= 1010){
    TinyTone(Note_B, 3);
  } else if(keyVal >= 505 && keyVal <= 520){
    TinyTone(Note_CS, 4);
  } else if(keyVal >= 3 && keyVal <= 30){
    TinyTone(Note_D, 4);
  } else {
    StopTone();
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
