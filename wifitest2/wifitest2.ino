#include <WiFi.h>
#include <ESPping.h>

// char* SSID = "Pixel_4304";
// char* PASS = "Stupendous4-Racecar";
// char* SSID = "Peter's iPhone";
// char* PASS = "12345678";
char* SSID = "jeff";
char* PASS = "12345678";

void ping_google() {
    IPAddress ip = IPAddress(8, 8, 8, 8);
    if (Ping.ping(ip)) {
        Serial.println(Ping.averageTime());
    } else {
        Serial.println("Failed to ping");
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

}
