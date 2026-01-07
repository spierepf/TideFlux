#include <osc.hpp>

#include <OSCMessage.h>
#include <WiFiUdp.h>

#ifndef TIDAL_OSC_UDP_PORT
#define TIDAL_OSC_UDP_PORT 6060
#endif

namespace osc
{
  WiFiUDP udp;
  const IPAddress broadcastIP(255, 255, 255, 255);

  OSCInput::OSCInput(const char *name) : msg("/ctrl"), name(name) {}

  void OSCInput::reportValue(float value)
  {
    Serial.printf("[OSC] Updating '%s' to %f\n", name, value);
    udp.beginPacket(broadcastIP, TIDAL_OSC_UDP_PORT);
    msg.empty().add(name).add(value).send(udp);
    udp.endPacket();
  }

  void OSCInput::reportValue(bool value)
  {
    Serial.printf("[OSC] Updating '%s' to %s\n", name, value ? "true" : "false");
    udp.beginPacket(broadcastIP, TIDAL_OSC_UDP_PORT);
    msg.empty().add(name).add(value).send(udp);
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
    float currentValue = analogRead(gpio) / 4095.0;

    if (abs(currentValue - lastReportedValue) > deadBand || ((currentValue == 0.0 || currentValue == 1.0) && currentValue != lastReportedValue))
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
    maxReading = std::max(reading, maxReading);
    minReading = std::min(reading, minReading);

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

  template <typename T, typename... Ptrs>
  std::vector<std::unique_ptr<T>> make_vector(Ptrs &&...ptrs)
  {
    std::vector<std::unique_ptr<T>> vec;
    (vec.emplace_back(std::forward<Ptrs>(ptrs)), ...);
    return vec;
  }

  std::vector<std::unique_ptr<OSCInput>> inputs = make_vector<OSCInput>(
      std::unique_ptr<OSCInput>(new AnalogOSCInput("slider", 32)),
      std::unique_ptr<OSCInput>(new TouchOSCInput("touch", 4, 21)));

  void setup()
  {
    for (auto &input : inputs)
      input->setup();
  }

  void loop()
  {
    for (auto &input : inputs)
      input->loop();
  }
}
