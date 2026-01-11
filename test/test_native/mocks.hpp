#pragma once

#include <gmock/gmock.h>

template <typename T>
class MockSource : public channel::Source<T>
{
public:
  MOCK_METHOD(T, get, (), (override));
};

template <typename T>
class MockSink : public channel::Sink<T>
{
public:
  MOCK_METHOD(void, recv, (T value), (override));
};
