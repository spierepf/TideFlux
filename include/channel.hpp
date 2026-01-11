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
}