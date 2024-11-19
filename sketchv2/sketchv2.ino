// Libraries
#include "SD.h" 
#include "SPI.h"
#include <ESP32Servo.h>
#include <WiFi.h>
#include "time.h"

const char* ssid = "wustl-guest-2.0";
const char* password = "";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

const int sd_pin = A5;        // SD module chip select pin
/*AdafruitIO_Feed *time_feed = io.feed("timefeed");
AdafruitIO_Feed *dist_feed = io.feed("distfeed");
*/
// Other stuff
int m_dist;
const int ms_sample = 1000;  

void get_distance() {
  m_dist = 6;
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Error");
    return;
  }
  Serial.println(&timeinfo, "%m:%d:%y %H:%M:%S");
}

const char* returnLocalTime() {
  static char timeString[20];
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "00:00:00 00:00:00";
  }
  strftime(timeString, sizeof(timeString), "%m:%d:%y %H:%M:%S", &timeinfo);
  return timeString;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SD.begin();
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  //io.run();
  
  delay(ms_sample);
  const char * data = returnLocalTime();
  Serial.println(data);
  struct tm timeinfo;
  get_distance();
  
  File data_file = SD.open("/datalog2.txt", FILE_APPEND);    // open the file
  if (data_file) {
    data_file.printf("%d , ", m_dist);
    data_file.println(data);
    data_file.close();
  } else {
    Serial.print("error opening ");
    Serial.println("/datalog2.txt");
  }
/*
  dist_feed->save(mm_dist);
  time_feed->save(returnLocalTime());
  t_sample = millis();

  // Wait for next sample
  wait_for_sample();*/
}