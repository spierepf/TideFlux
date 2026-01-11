#include <ArduinoOTA.h>

namespace ota
{
  uint32_t last_ota_time = 0;

  void setup()
  {
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
#ifdef HOSTNAME
    ArduinoOTA.setHostname(HOSTNAME);
#endif

    // Password can be set with plain text (will be hashed internally)
    // The authentication uses PBKDF2-HMAC-SHA256 with 10,000 iterations
#ifdef OTA_PASSWORD
    ArduinoOTA.setPassword(OTA_PASSWORD);
#endif

    // Or set password with pre-hashed value (SHA256 hash of "admin")
    // SHA256(admin) = 8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918
#ifdef OTA_PASSWORD_HASH
    ArduinoOTA.setPasswordHash(OTA_PASSWORD_HASH);
#endif

    ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
          type = "sketch";
        } else {  // U_SPIFFS
          type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        if (millis() - last_ota_time > 500) {
          Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
          last_ota_time = millis();
        }
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
          Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
          Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
          Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
          Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
          Serial.println("End Failed");
        }
      });

    ArduinoOTA.begin();
  }

  void loop()
  {
    ArduinoOTA.handle();
  }
}
