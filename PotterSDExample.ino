// Last problem: 27 not recording voltage of battery... or anything..
// everything else works.

// Change BATTERY PIN TO 13?
// A3 does work... anything else on the battery side does not work.
// I think this would need to be a jumper situation. female-female jumper from BAT to A3, for example.


// Still to do
//    1. add ultrasonic to program
//    2. adjust pin numbers so that PCB is feasible
//    3. add Adafruit.io functionality... or GOOGLE SHEET!

// New connectons

// RTC
//    VCC to 3.3V rail
//    GND to GND
//    SDA to SDA
//    SCL to SCL

// SD
//    GND to GND
//    VCC to USB!!!
//    MISO to MI
//    MOSI to MO
//    SCK to SCK
//    CS to pin A5

// BUZZER
//    + to pin 14

// File name needs to have an initial forward slash! WOW!


// Libraries
#include "config.h"
#include <ESP32Servo.h>
#include "SD.h"
#include "SPI.h"
#include "RTClib.h"
RTC_DS3231 myRTC;             // real-time clock (RTC) object

// Pin numbers
const int trigger_pin = 33;   // (same as A9)    
const int echo_pin = 32;      // (same as A7)
const int buzzer_pin = 14;    // piezoelectric buzzer, PWM on startup
const int sd_pin = A5;        // (same as 4) SD module chip select pin
const int pot_pin = A2;       // (same as 34, only input)
const int bat_pin = A13;      // (internal connection) battery voltage

// Other stuff
float dt_echo;                // [millisec] time for pulse/echo to return
float cm_dist;
int mm_dist;
const int ms_sample = 10000;  // [1/1000 sec] time between samples
const long ms_time_out = ms_sample*0.5;   // [microsec] give up if echo takes longer than this
const long time_out = 1000*ms_time_out;   // [microsec] give up if echo takes longer than this
const int ADC_max = 4096;     // 12-bit ADC
const bool wait_for_serial = true;
long t_sample;                // [1/1000 sec] time instant of latest sample

AdafruitIO_Feed *pot_feed = io.feed("potfeed");
AdafruitIO_Feed *bat_feed = io.feed("batfeed");
AdafruitIO_Feed *dist_feed = io.feed("distfeed");


void make_disconnect_sound() {
  tone(buzzer_pin, 440, 110); // A
  delay(125);
  tone(buzzer_pin, 349, 110); // F
  delay(125);
  tone(buzzer_pin, 330, 110); // E 
}


void make_connect_sound() {
  tone(buzzer_pin, 523, 125); // C
  delay(250);
  tone(buzzer_pin, 440, 110); // A
  delay(125);
  tone(buzzer_pin, 349, 110); // F
  delay(125);
  tone(buzzer_pin, 659, 110); // E
}


void get_distance() {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);   
  dt_echo = pulseIn(echo_pin, HIGH, time_out);
  if (dt_echo) {
    cm_dist = (dt_echo*.0343)/2.0;      // [cm] from https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380
  } else {
    cm_dist = (ms_time_out*.0343)/2.0;  // distance is larger than this
  }
  mm_dist = round(cm_dist*10);
}


void wait_for_sample() {
  for (;;) {
    if (millis() - t_sample >= ms_sample) {break;}
  }
}


void setup() {
  Serial.begin(115200); 
  if (wait_for_serial) {
    while(! Serial);      // wait for the serial monitor to open 
  }

  // Connect to io.adafruit.com.
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Wait for the connection to complete...
  while(io.status() < AIO_CONNECTED) {     
    Serial.print(".");
    delay(500);
  }

  // Connection was successful!
  Serial.println();
  Serial.println(io.statusText());

  pinMode(buzzer_pin, OUTPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(pot_pin, INPUT);
  pinMode(bat_pin, INPUT);
  pinMode(sd_pin, OUTPUT);
  digitalWrite(sd_pin, LOW);   // tell SD module that we're talking to it... SPI protocol

  myRTC.begin();

  if (!SD.begin()) {          // cannot find SD card... let the user know this!
    while (true) {
      Serial.println("Card Mount Failed");
      make_disconnect_sound();
      delay(2000);            // wait 2 seconds, then repeat ad infinitum
    }
  } else {                    // SD card was found!
    make_connect_sound();
  }
}


void loop(){
  // io.run(); is required for all sketches that use Adafruit IO.
  // It should always be present at the top of your loop function.
  // It keeps the client connected to io.adafruit.com and
  // processes any incoming data.
  io.run();

  // Get sample values and time
  int pot_val = analogRead(pot_pin);
  int bat_val = analogRead(bat_pin);
  get_distance();
  DateTime now = myRTC.now();
  Serial.printf("%d,%d,%d,%d-%02d-%02d,%02d:%02d:%02d\n", mm_dist, pot_val, bat_val, now.year(), now.month(), now.day(), now.hour() , now.minute(), now.second());
  
  // Store data to SD card
  File data_file = SD.open("/datalog.txt", FILE_APPEND);    // open the file
  if (data_file) {
    data_file.printf("%d,%d,%d,%d-%02d-%02d,%02d:%02d:%02d\n", mm_dist, pot_val, bat_val, now.year(), now.month(), now.day(), now.hour() , now.minute(), now.second());
    data_file.close();
  } else {
    Serial.print("error opening ");
    Serial.println("/datalog.txt");
  }

  // Store data to cloud
  dist_feed->save(mm_dist);
  pot_feed->save(pot_val);
  bat_feed->save(bat_val);
  t_sample = millis();

  // Wait for next sample
  wait_for_sample();
}