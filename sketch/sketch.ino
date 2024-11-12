// Authors: Hayden Schroeder and Erica Shi
// Description: Take data from sensors and store it locally as well as have web access to the data.

// Libraries:
// - https://github.com/dvarrel/ESPping/tree/main

// #include <ETH.h>
#include <WiFi.h>
#include <ESPping.h>
// Libraries
#include "SD.h" 
#include "SPI.h"
#include "RTClib.h"
#include <HTTPClient.h>
#include "sketch.h"

RTC_DS3231 myRTC;    

// Pin numbers
//const int buzzer_pin = 14;    // piezoelectric buzzer for making sounds
const int sd_pin = A5;        // SD module chip select pin
//const int pot_pin = A2;       // good ol' potentiometer voltage

// Other stuff
const int ms_sample = 2000;   // [1/1000 sec] time between samples
long t_sample;
int m_dist;
char* SSID = "Pixel_4304";
char* PASS = "Stupendous4-Racecar";


void get_distance() {
  m_dist = 5;
}

void wait_for_sample() {
  for (;;) {
    if (millis() - t_sample >= ms_sample) {break;}
  }
}

void ping_google() {
    IPAddress ip = IPAddress(8, 8, 8, 8);
    if (Ping.ping(ip)) {
        Serial.println(Ping.averageTime());
    } else {
        Serial.println("Failed to ping");
    }
}

char * get_box_token(char * client_id, char * client_secret) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("https://api.box.com/oauth2/token");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String body = "grant_type=client_credentials&client_id=" + String(client_id) + "&client_secret=" + String(client_secret);
        int httpResponseCode = http.POST(body);
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
            return response.c_str();
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
            return NULL;
        }
        http.end();
    } else {
        Serial.println("WiFi Disconnected");
        return NULL;
    }
}

void upload_to_box(char *filename, char *token) {
    if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://upload.box.com/api/2.0/files/content");
    http.addHeader("Authorization", "Bearer " + String(token));
    http.addHeader("Content-Type", "multipart/form-data");
    String attributes = "{\"name\":\"" + String(filename) + "\", \"parent\":{\"id\":\"0\"}}";
    http.addHeader("attributes", attributes);
    File file = SD.open(filename, "r"); // TODO: change to SD.open
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    int httpResponseCode = http.sendRequest("POST", &file, file.size());

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    file.close();
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(SSID, PASS);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

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

void loop() {
  // put your main code here, to run repeatedly:
    ping_google();
    // delay(1000);
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