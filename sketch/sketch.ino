// Authors: Hayden Schroeder and Erica Shi
// Description: Take data from sensors and store it locally as well as have web access to the data.

// Libraries
#include "SD.h" 
#include "SPI.h"
#include "RTClib.h"
RTC_DS3231 myRTC;    

// Pin numbers
//const int buzzer_pin = 14;    // piezoelectric buzzer for making sounds
const int sd_pin = A5;        // SD module chip select pin
//const int pot_pin = A2;       // good ol' potentiometer voltage

// Other stuff
const int ms_sample = 2000;   // [1/1000 sec] time between samples
long t_sample;
int m_dist;


void get_distance() {
  m_dist = 5;
}

void wait_for_sample() {
  for (;;) {
    if (millis() - t_sample >= ms_sample) {break;}
  }
}

void setup() {
  Serial.begin(115200); 
  //pinMode(pot_pin, INPUT);
  //pinMode(buzzer_pin, OUTPUT);
  pinMode(sd_pin, OUTPUT);
  digitalWrite(sd_pin, LOW);   // tell SD module that we're talking to it... SPI protocol

  myRTC.begin();
  if (!SD.begin()) {          // cannot find SD card... let the user know this!
    while (true) {
      Serial.println("Card Mount Failed");
      delay(2000);            // wait 2 seconds, then repeat ad infinitum
    }
  } else {                    // SD card was found!
    Serial.println("Card Mount Found");
  }
}



void loop(){
  //int pot_val = analogRead(pot_pin);
  get_distance();
  DateTime now = myRTC.now();
  
  File data_file = SD.open("/datalog.txt", FILE_APPEND);    // open the file
  if (data_file) {
    data_file.printf("%d, %02d:%02d:%d %02d:%02d:%02d\n", m_dist, now.month(), now.day(), now.year(), now.hour() , now.minute(), now.second());
    data_file.close();
  } else {
    Serial.print("error opening ");
    Serial.println("/datalog.txt");
  }
  delay(ms_sample);    // actual sample time is SLIGHTLY more than this
}
