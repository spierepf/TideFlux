#include <gmock/gmock.h>

#include "channel.hpp"

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

TEST(SourceTest, get)
{
  MockSource<float> source;
  EXPECT_CALL(source, get()).WillRepeatedly(testing::Return(42.0f));

  EXPECT_EQ(42.0, source.get());
}

TEST(SinkTest, recv)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(42.0f));

  sink.recv(42.0f);
}

TEST(ChannelTest, TransfersValueFromSourceToSinkWhenLoopCalled)
{
  MockSource<float> source;
  EXPECT_CALL(source, get()).WillRepeatedly(testing::Return(42.0f));

  MockSink<float> sink;
  EXPECT_CALL(sink, recv(42.0f));

  channel::Channel<float> channel(source, sink);

  channel.loop();
}
