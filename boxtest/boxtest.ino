#include <WiFi.h>
#include <ESPping.h>
#include "boxtest.h"
#include <HTTPClient.h>
#include <SD.h>

// char* SSID = "Pixel_4304";
// char* PASS = "Stupendous4-Racecar";
// char* SSID = "Peter's iPhone";
// char* PASS = "12345678";
// char* SSID = "jeff";
// char* PASS = "12345678";
char* SSID = "wustl-guest-2.0";
char* PASS = "";

BoxTest boxTest;

void ping_google() {
    IPAddress ip = IPAddress(8, 8, 8, 8);
    if (Ping.ping(ip)) {
        Serial.println(Ping.averageTime());
    } else {
        Serial.println("Failed to ping");
    }
}

const char * get_box_token(const char * client_id, const char * client_secret) {
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
}

void loop() {
  // put your main code here, to run repeatedly:
    ping_google();
    const char * token = get_box_token(boxTest.CLIENT_ID, boxTest.CLIENT_SECRET);
    Serial.println(token);
}
