#include <limits>
#include <cmath>

#include "channel.hpp"

namespace channel
{
  Deadband::Deadband(Sink<float> &dest, float min, float max, float radius) : dest(dest), min(min), max(max), radius(radius) {}

  void Deadband::recv(float value)
  {
    if (fabs(value - lastReportedValue) > radius
        || (value != lastReportedValue && (value == min || value == max)) 
        || std::isnan(lastReportedValue))
    {
      dest.recv(value);
      lastReportedValue = value;
    }
  }
}