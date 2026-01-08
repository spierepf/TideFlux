#include <osc.hpp>

#include <OSCMessage.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#ifndef TIDAL_OSC_UDP_PORT
#define TIDAL_OSC_UDP_PORT 6060
#endif

namespace osc
{
  const int ANALOG_RESOLUTION = 12;
  constexpr float ADC_MAX = (1 << ANALOG_RESOLUTION) - 1;

  WiFiUDP udp;
  IPAddress broadcastIP;

  OSCInput::OSCInput(const char *name) : name(name) {}

  void OSCInput::reportValue(float value)
  {
    Serial.printf("[OSC] Updating '%s' to %f\n", name, value);
    udp.beginPacket(broadcastIP, TIDAL_OSC_UDP_PORT);
    OSCMessage msg("/ctrl");
    msg.add(name).add(value).send(udp);
    udp.endPacket();
  }

  void OSCInput::reportValue(bool value)
  {
    Serial.printf("[OSC] Updating '%s' to %s\n", name, value ? "true" : "false");
    udp.beginPacket(broadcastIP, TIDAL_OSC_UDP_PORT);
    OSCMessage msg("/ctrl");
    msg.add(name).add(value).send(udp);
    udp.endPacket();
  }

  AnalogOSCInput::AnalogOSCInput(const char *name, int gpio, float deadBand) : OSCInput(name), gpio(gpio), deadBand(deadBand) {}

  void AnalogOSCInput::setup()
  {
    Serial.printf("Configuring analog input '%s' on gpio %i\n", name, gpio);
    pinMode(gpio, INPUT);
  }

  void AnalogOSCInput::loop()
  {
    float currentValue = analogRead(gpio) / ADC_MAX;

    if (fabs(currentValue - lastReportedValue) > deadBand
      || ((currentValue <= 0.0 || currentValue >= 1.0) && currentValue != lastReportedValue)
      || std::isnan(lastReportedValue))
    {
      reportValue(currentValue);
      lastReportedValue = currentValue;
    }
  }

  TouchOSCInput::TouchOSCInput(const char *name, int touchGPIO, int indicatorGPIO) : OSCInput(name), touchGPIO(touchGPIO), indicatorGPIO(indicatorGPIO) {}

  void TouchOSCInput::setup()
  {
    Serial.printf("Configuring touch input '%s' on gpio %i with indicator on gpio %i\n", name, touchGPIO, indicatorGPIO);
    pinMode(touchGPIO, INPUT);
    pinMode(indicatorGPIO, OUTPUT);
    maxReading = minReading = touchRead(touchGPIO);
  }

  void TouchOSCInput::loop()
  {
    touch_value_t reading = touchRead(touchGPIO);

    touch_value_t decay = (maxReading - minReading) >> 5;
    maxReading = std::max(reading, (touch_value_t)(maxReading - decay));
    minReading = std::min(reading, (touch_value_t)(minReading + decay));

    if ((maxReading - minReading) > (maxReading >> 2))
    {
      bool currentValue = reading < ((minReading + maxReading) >> 1);
      if (!hasReported || currentValue != lastReportedValue)
      {
        reportValue(currentValue);
        lastReportedValue = currentValue;
        digitalWrite(indicatorGPIO, currentValue);
        hasReported = true;
      }
    }
  }

  bool wifiConnected = false;
  std::vector<std::unique_ptr<OSCInput>> inputs;
  void setup()
  {
    inputs.emplace_back(std::unique_ptr<OSCInput>(new AnalogOSCInput("slider", 32)));
    inputs.emplace_back(std::unique_ptr<OSCInput>(new TouchOSCInput("touch", 4, 21)));

    analogReadResolution(ANALOG_RESOLUTION);

    for (auto &input : inputs)
      input->setup();
  }

  void loop()
  {
    if (!wifiConnected && WiFi.status() == WL_CONNECTED)
    {
      wifiConnected = true;
      broadcastIP = WiFi.localIP();
      broadcastIP[3] = 255;
      udp.begin(0);
    }

    if (wifiConnected && WiFi.status() != WL_CONNECTED)
    {
      wifiConnected = false;
    }

    if (wifiConnected)
    {
      for (auto &input : inputs)
        input->loop();
    }
  }
}
