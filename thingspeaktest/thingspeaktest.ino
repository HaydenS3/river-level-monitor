#include <WiFi.h>
#include <ESPping.h>
#include <HTTPClient.h>
#include <SD.h>

char* SSID = "wustl-guest-2.0";
char* PASS = "";
unsigned long counter = 0;

void write_channel_feed(const char * data) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("https://api.thingspeak.com/update?api_key=KEYHERE&field1=" + String(data));
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Error on sending GET: ");
            Serial.println(httpResponseCode);
        }
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
  char buffer[10];
  sprintf(buffer, "%d", counter);
  Serial.println(buffer);
  write_channel_feed(buffer);
    counter++;
    delay(1000);
}
