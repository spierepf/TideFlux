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

  void setup();
  void loop();
}