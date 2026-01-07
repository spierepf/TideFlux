#pragma once

#include <vector>
#include <memory>

#include <OSCMessage.h>

namespace osc
{
  class OSCInput
  {
  public:
    OSCInput(const char *name);
    virtual ~OSCInput() = default;

    virtual void setup() = 0;
    virtual void loop() = 0;

  protected:
    void reportValue(float value);
    void reportValue(bool value);
    const char *name;

  private:
    OSCMessage msg;
  };

  class AnalogOSCInput : public OSCInput
  {
  public:
    AnalogOSCInput(const char *name, int port, float deadBand = 0.02);
    virtual ~AnalogOSCInput() = default;

    virtual void setup();
    virtual void loop();

  private:
    int gpio;
    float deadBand;
    float lastReportedValue = -1.0;
  };

  class TouchOSCInput : public OSCInput
  {
  public:
    TouchOSCInput(const char *name, int touchGPIO, int indicatorGPIO);
    virtual ~TouchOSCInput() = default;

    virtual void setup();
    virtual void loop();

  private:
    int touchGPIO;
    int indicatorGPIO;
    touch_value_t maxReading, minReading;
    bool hasReported = false;
    bool lastReportedValue;
  };

  void setup();
  void loop();
}
