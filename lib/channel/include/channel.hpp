#pragma once

namespace channel
{
  template <typename T>
  class Source
  {
  public:
    virtual ~Source() noexcept = default;
    virtual T get() = 0;
  };

  template <typename T>
  class Sink
  {
  public:
    virtual ~Sink() noexcept = default;
    virtual void recv(T value) = 0;
  };

  template <typename T>
  class Channel
  {
  public:
    Channel(Source<T> &source, Sink<T> &sink) : source(source), sink(sink) {}
    void loop()
    {
      sink.recv(source.get());
    }

  private:
    Source<T> &source;
    Sink<T> &sink;
  };

  class Deadband : public Sink<float>
  {
  public:
    Deadband(Sink<float> &dest, float min = -std::numeric_limits<float>::max(), float max = std::numeric_limits<float>::max(), float radius = 0.0);
    virtual void recv(float value) override;

  private:
    float lastReportedValue = NAN;
    float min;
    float max;
    float radius;
    Sink<float> &dest;
  };
}