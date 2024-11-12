// Authors: Hayden Schroeder and Erica Shi
// Description: Take data from sensors and store it locally as well as have web access to the data.

// Libraries:
// - https://github.com/dvarrel/ESPping/tree/main

// #include <ETH.h>
#include <WiFi.h>
#include <ESPping.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin("Pixel_4304", "Stupendous4-Racecar");
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
    // delay(1000);
}


// Code for LoRa mode on ESP32
// void init_wifi(wifi_mode_t mode)
// {
//     const uint8_t protocol = WIFI_PROTOCOL_LR;
//     tcpip_adapter_init();
//     ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
//     ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
//     ESP_ERROR_CHECK( esp_wifi_set_mode(mode) );
//     wifi_event_group = xEventGroupCreate();

//     if (mode == WIFI_MODE_STA) {
//         ESP_ERROR_CHECK( esp_wifi_set_protocol(WIFI_IF_STA, protocol) );
//         wifi_config_t config = {
//             .sta = {
//                 .ssid = ap_name,
//                 .password = pass,
//                 .bssid_set = false
//             }
//         };
//         ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &config) );
//         ESP_ERROR_CHECK( esp_wifi_start() );
//         ESP_ERROR_CHECK( esp_wifi_connect() );

//         xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
//                             false, true, portMAX_DELAY);
//         ESP_LOGI(TAG, "Connected to AP");
//     } else {
//         ESP_ERROR_CHECK( esp_wifi_set_protocol(WIFI_IF_AP, protocol) );
//         wifi_config_t config = {
//             .ap = {
//                 .ssid = ap_name,
//                 .password = pass,
//                 .ssid_len = 0,
//                 .authmode = WIFI_AUTH_WPA_WPA2_PSK,
//                 .ssid_hidden = false,
//                 .max_connection = 3,
//                 .beacon_interval = 100,
//             }
//         };
//         ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &config) );
//         ESP_ERROR_CHECK( esp_wifi_start() );
//     }
// }

void ping_google() {
    IPAddress ip = IPAddress(8, 8, 8, 8);
    if (Ping.ping(ip)) {
        Serial.println(Ping.averageTime());
    } else {
        Serial.println("Failed to ping");
    }
}