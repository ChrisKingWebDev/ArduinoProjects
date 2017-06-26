#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>
#include <Timezone.h>

const char* ssid = "strikeforce";
const char* password = "superman";
const char* host = "www.chriskingwebdev.com";
String Months[] = {"Jan","Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct","Nov", "Dec" };
int prevMinute = -1;
int weekdayAlarmHour = -1;
int weekdayAlarmMinute = -1;
int saturdayAlarmHour = -1;
int saturdayAlarmMinute = -1;
int sundayAlarmHour = -1;
int sundayAlarmMinute = -1;
String timeZoneName;

int alarmDuration = 40;
int alarmRemainDuration = 10;

ESP8266WebServer server(80);

MDNSResponder mdns;

#define PIN      5
#define STEPS 30.0
#define N_LEDS 120
long totalSteps;
long currentStep;
int colors = 7;

// float ledInfo[N_LEDS][4];
int ledCount[N_LEDS];

uint16_t ledStep;
uint16_t colorStep;

long totalTime;
double mills_per_step;
double start_millis;

float darkBlue[3] = {0, 0, 20};
float darkPurple[3] = {20, 0, 20};
float maroon[3] = {19, 7, 13};
float darkRed[3] = {50, 0, 0};
float orange[3] = {128, 51, 0};
float yellow[3] = {150, 100, 0};
float white[3] = {150, 150, 150};
float currentColor[3];
float cRed = 0;
float cGreen = 0;
float cBlue = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  strip.begin();

  resetLeds();

  WiFi.begin(ssid, password);

  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);

    //a flashing LED means the ESP is not connected
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }

  mdns.begin("esp8266", WiFi.localIP());

  server.on("/info", [](){
    long serverTime = getTimeFromServer();
    setTime(serverTime);
    String output = "<html><body><h1>Sunrise Alarm Info</h1>Current Time: ||time||<br/>Time Zone: ||timeZone||<br/><br/>Cur Weekday: ||curWeekday||<br/><br/>Weekday Alarm: ||weekdayAlarm||<br/>Saturday Alarm: ||saturdayAlarm||<br/>Sunday Alarm: ||sundayAlarm||<br/><br/>Alarm Duration: ||alarmDuration||<br/>Alarm Remain Duration: ||alarmRemainDuration||</body></html>";

    time_t t = now();
    int curHour = hour(t);
    int curMinute = minute(t);
    int curSecond = second(t);
    int curWeekday = weekday(t);

    output.replace("||time||",String(curHour) + ":" + curMinute + ":" + curSecond);
    output.replace("||timeZone||",String(timeZoneName));
    output.replace("||curWeekday||", String(curWeekday));
    output.replace("||weekdayAlarm||",String(weekdayAlarmHour) + ":" + weekdayAlarmMinute);
    output.replace("||saturdayAlarm||",String(saturdayAlarmHour) + ":" + saturdayAlarmMinute);
    output.replace("||sundayAlarm||",String(sundayAlarmHour) + ":" + sundayAlarmMinute);
    output.replace("||alarmDuration||", String(alarmDuration));
    output.replace("||alarmRemainDuration||", String(alarmRemainDuration));

    server.send(200, "text/html", output);
  });

  server.onNotFound([](){
    String path = server.uri();
    if (path.indexOf("/setalarm") >= 0) {

      String output = "<html><body><h1>Setting Alarm</h1>Alarm Type: ||alarmType||<br/>Alarm Time: ||alarmTime||</body></html>";
      String alarmType = "";

      String pathRemainder = path.substring(9, path.length());
      //Serial.println(pathRemainder);
      if (pathRemainder.indexOf("/weekday") >= 0){
        alarmType = "weekday";
        pathRemainder = pathRemainder.substring(pathRemainder.indexOf("/weekday") + 8, pathRemainder.length());
      } else if (pathRemainder.indexOf("/saturday") >= 0){
        alarmType = "saturday";
        pathRemainder = pathRemainder.substring(pathRemainder.indexOf("/saturday") + 9, pathRemainder.length());
      } else if (pathRemainder.indexOf("/sunday") >= 0){
        alarmType = "sunday";
        pathRemainder = pathRemainder.substring(pathRemainder.indexOf("/sunday") + 7, pathRemainder.length());
      }
      String hourParam = pathRemainder.substring(1, pathRemainder.indexOf(":"));
      String minuteParam = pathRemainder.substring(pathRemainder.indexOf(":") + 1, pathRemainder.length());

      if (alarmType == "weekday") {
          weekdayAlarmHour = hourParam.toInt();
          weekdayAlarmMinute = minuteParam.toInt();
      } else if (alarmType == "saturday") {
          saturdayAlarmHour = hourParam.toInt();
          saturdayAlarmMinute = minuteParam.toInt();
      } else {
        sundayAlarmHour = hourParam.toInt();
        sundayAlarmMinute = minuteParam.toInt();
      }

      output.replace("||alarmType||",alarmType);
      output.replace("||alarmTime||",hourParam + ":" + minuteParam);

      server.send(200, "text/html", output);
    } else if (path.indexOf("/setduration") >= 0) {
      String output = "<html><body><h1>Setting Alarm Duration</h1>Alarm Duration: ||alarmDuration||</body></html>";
      String durationParam = path.substring(13, path.length());
      //Serial.println(durationParam);
      alarmDuration = durationParam.toInt();
      output.replace("||alarmDuration||",durationParam);
      server.send(200, "text/html", output);
    } else if (path.indexOf("/setremain") >= 0) {
      String output = "<html><body><h1>Setting Alarm Remain Duration</h1>Alarm Remain Duration: ||alarmRemainDuration||</body></html>";
      String remainDurationParam = path.substring(11, path.length());
      //Serial.println(remainDurationParam);
      alarmRemainDuration = remainDurationParam.toInt();
      output.replace("||alarmRemainDuration||",remainDurationParam);
      server.send(200, "text/html", output);
    } else {
      server.send(404, "text/html", "Command Not Found");
    }
  });

  server.begin();
  long serverTime = getTimeFromServer();
  setTime(serverTime);
  setSyncProvider(getTimeFromServer);
  setSyncInterval(60 * 60);

  totalSteps = STEPS * N_LEDS * colors;

  currentStep = -1;
}

void loop() {
  server.handleClient();
  time_t t = now();
  int curHour = hour(t);
  int curMinute = minute(t);
  int curSecond = second(t);
  int curWeekday = weekday(t);

  if (curSecond == 0 && curMinute != prevMinute) {
    Serial.print("New Minute:");
    Serial.print(curHour);
    Serial.print(":");
    Serial.print(curMinute);
    Serial.print(":");
    Serial.println(curSecond);
    prevMinute = curMinute;
    if (curWeekday == 7 && saturdayAlarmHour == curHour && saturdayAlarmMinute == curMinute) {
      Serial.println("Start Saturday Alarm Now");
      initSunrise();
    } else if (curWeekday == 1 && sundayAlarmHour == curHour && sundayAlarmMinute == curMinute) {
      Serial.println("Start Sunday Alarm Now");
      initSunrise();
    } else if (curWeekday != 7 && curWeekday != 1 && weekdayAlarmHour == curHour && weekdayAlarmMinute == curMinute) {
      Serial.println("Start Weekday Alarm Now");
      initSunrise();
    }
  }
  doSunrise();
}

long getTimeFromServer() {
   time_t pacific, utc;
   //Serial.print("connecting to ");
   //Serial.println(host);

   // Use WiFiClient class to create TCP connections
   WiFiClient client;
   const int httpPort = 80;
   if (!client.connect(host, httpPort)) {
    //Serial.println("connection failed");
    return 0;
   }
   client.print(String("GET ") + "/sunrise_clock.txt HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

   unsigned long timeout = millis();
   while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      //Serial.println(">>> Client Timeout !");
      client.stop();
      return 0;
    }
    }

     while(client.available()){
      String line = client.readStringUntil('\r');
      // Serial.println(line);
      if(line.indexOf("weekdayAlarmHour") > -1){
        weekdayAlarmHour = line.substring(line.indexOf("weekdayAlarmHour")+17,line.indexOf("weekdayAlarmHour")+19).toInt();
      }
      if(line.indexOf("weekdayAlarmMinute") > -1){
        weekdayAlarmMinute = line.substring(line.indexOf("weekdayAlarmMinute")+19,line.indexOf("weekdayAlarmMinute")+21).toInt();
      }
      if(line.indexOf("saturdayAlarmHour") > -1){
        saturdayAlarmHour = line.substring(line.indexOf("saturdayAlarmHour")+18,line.indexOf("saturdayAlarmHour")+20).toInt();
      }
      if(line.indexOf("saturdayAlarmMinute") > -1){
        saturdayAlarmMinute = line.substring(line.indexOf("saturdayAlarmMinute")+20,line.indexOf("saturdayAlarmMinute")+22).toInt();
      }
      if(line.indexOf("sundayAlarmHour") > -1){
        sundayAlarmHour = line.substring(line.indexOf("sundayAlarmHour")+16,line.indexOf("sundayAlarmHour")+18).toInt();
      }
      if(line.indexOf("sundayAlarmMinute") > -1){
        sundayAlarmMinute = line.substring(line.indexOf("sundayAlarmMinute")+18,line.indexOf("sundayAlarmMinute")+20).toInt();
      }
      if(line.indexOf("alarmDuration") > -1){
        alarmDuration = line.substring(line.indexOf("alarmDuration")+14,line.indexOf("alarmDuration")+16).toInt();
      }
      if(line.indexOf("alarmRemainDuration") > -1){
        alarmRemainDuration = line.substring(line.indexOf("alarmRemainDuration")+20,line.indexOf("alarmRemainDuration")+22).toInt();
      }
      if(line.indexOf("Date:") > -1) {
        //Serial.println("Found Date: " + line);
        String dayStr = line.substring(12,14);
        String monthStr = line.substring(15,18);
        int monthsInt;

        for(int i = 0; i < sizeof(Months); i++){
          if(Months[i] == monthStr){
            monthsInt = i + 1;
          }
        }

        String yearStr = line.substring(19,23);
        String hours = line.substring(24,26);
        //Serial.println(hours);
        String minutes = line.substring(27,29);
        //Serial.println(minutes);
        String seconds = line.substring(30,32);
        //Serial.println(seconds);
        String timeString = line.substring(6,line.length());
        setTime(hours.toInt(),minutes.toInt(),seconds.toInt(),dayStr.toInt(),monthsInt,yearStr.toInt());

        TimeChangeRule *tcr;
        TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};  //UTC - 7 hours
        TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};   //UTC - 8 hours
        Timezone usPacific(usPDT, usPST);
        utc = now();    //current time from the Time Library
        pacific = usPacific.toLocal(utc, &tcr);
        //Serial.print("The time zone is: ");
        //Serial.println(tcr -> abbrev);
        timeZoneName = tcr -> abbrev;
      }
    }
    return pacific;
}

void initSunrise() {

    Serial.println("Starting sunrise");

    Serial.println("Alarm Duration: " + String(alarmDuration));

    totalTime = (alarmDuration * 60 * 1000L);
    Serial.println("Total Time: " + String(totalTime));
    mills_per_step = totalTime / totalSteps;

    Serial.println("Total Steps: " + String(totalSteps));
    Serial.println("Mills per step: " + String(mills_per_step));

    resetLeds();
    resetStripColor();

    currentColor[0] = 0;
    currentColor[1] = 0;
    currentColor[2] = 0;
    cRed = 0;
    cGreen = 0;
    cBlue = 0;

    start_millis = millis();
    currentStep = 0;

    Serial.println("start_millis: " + String(start_millis));
    Serial.println("currentStep: " + String(currentStep));
}
void doSunrise() {
  double now = millis();
  if(currentStep >= 0 && currentStep < totalSteps && (now >= start_millis + (currentStep * mills_per_step))) {
    int i = -1;
    ledStep = floor(currentStep / N_LEDS);
    do {
      int randNumber = random(N_LEDS);
      if(ledCount[randNumber] <= ledStep){
      //if(ledCount[randNumber] < (currentStep / N_LEDS)){
        i = randNumber;
      }
    } while ( i == -1 );
    // int i = currentStep % N_LEDS;


      // float cRed = ledInfo[i][0];
      // float cGreen = ledInfo[i][1];
      // float cBlue = ledInfo[i][2];

      ledCount[i]++;
      //ledStep = ledCount[i];
      //colorStep;
      if (currentStep % N_LEDS == 0) {
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
          currentColor[0] = 150;
          currentColor[1] = 150;
          currentColor[2] = 150;
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
      }

      if (i == 1) {
        //Serial.println("Step: " + String(ledStep) + " Color: " + String(cRed) + ":" + String(cGreen) + ":" + String(cBlue));
      }

      strip.setPixelColor(i, strip.Color(cRed, cGreen, cBlue));
      strip.show();

      currentStep++;

      // ledInfo[i][0] = cRed;
      // ledInfo[i][1] = cGreen;
      // ledInfo[i][2] = cBlue;
      ledCount[i] = ledStep;

      // delay(mills_per_step);
  }
  if(currentStep == totalSteps && (now >= start_millis + (currentStep * mills_per_step) + (alarmRemainDuration * 60 * 1000L))){
    Serial.println("Done Steps");
    currentStep = -1;
    resetStripColor();
  }


  // delay(alarmRemainDuration * 60 * 1000L);
  //
  //

  //resetLeds();
}

void resetLeds() {
  Serial.println("Resetting LEDS");
  for(int i = 0; i <= N_LEDS; i++){
    // ledInfo[i][0] = 0;
    // ledInfo[i][1] = 0;
    // ledInfo[i][2] = 0;
    ledCount[i] = 0;
  }
}

void resetStripColor() {
  for(int k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor(k, strip.Color(0, 0, 0));
  }
  strip.show();
}
