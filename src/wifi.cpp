#include <WiFi.h>

#ifndef WIFI_SSID
#define WIFI_SSID "your-ssid"
#define WIFI_PSK "your-password"
#endif

const char *ssid = WIFI_SSID;
const char *password = WIFI_PSK;

namespace wifi {
  void setup() {
    Serial.println();
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    int tryDelay = 500;
    int numberOfTries = 20;

    while (true) {
      switch (WiFi.status()) {
        case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
        case WL_CONNECT_FAILED:
          Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
          return;
          break;
        case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
        case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
        case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
        case WL_CONNECTED:
          Serial.println("[WiFi] WiFi is connected!");
          Serial.print("[WiFi] IP address: ");
          Serial.println(WiFi.localIP());
          return;
          break;
        default:
          Serial.print("[WiFi] WiFi Status: ");
          Serial.println(WiFi.status());
          break;
      }
      delay(tryDelay);

      if (numberOfTries <= 0) {
        Serial.print("[WiFi] Failed to connect to WiFi!");
        WiFi.disconnect();
        return;
      } else {
        numberOfTries--;
      }
    }
  }

  void loop() {

  }
}
