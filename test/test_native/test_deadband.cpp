#include <gmock/gmock.h>

#include "channel.hpp"

#include "mocks.hpp"


TEST(Deadband, AlwaysForwardsTheFirstValueItReceives)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(42.0f)).Times(1);

  channel::Deadband deadband(sink);

  deadband.recv(42.0);
}

TEST(Deadband, DoesNotForwardTheSecondValueItReceivesIfItMatchesTheFirst)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(42.0f)).Times(1);

  channel::Deadband deadband(sink);

  deadband.recv(42.0);
  deadband.recv(42.0);
}

TEST(Deadband, ForwardsTheSecondValueItReceivesIfItDiffersFromTheFirst)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(42.0f)).Times(1);
  EXPECT_CALL(sink, recv(43.0f)).Times(1);

  channel::Deadband deadband(sink);

  deadband.recv(42.0);
  deadband.recv(43.0);
}

TEST(Deadband, DoesNotForwardTheSecondValueItReceivesIfItIsInsufficientlyDifferentFromTheFirst)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(42.0f)).Times(1);

  channel::Deadband deadband(sink, 0.0f, 100.0f, 2.0f);

  deadband.recv(42.0);
  deadband.recv(43.0);
}

TEST(Deadband, ForwardsWhenReachingMinForTheFirstTimeEvenWhenWithinDeadband)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(1.0f)).Times(1);
  EXPECT_CALL(sink, recv(0.0f)).Times(1);

  channel::Deadband deadband(sink, 0.0f, 100.0f, 2.0f);

  deadband.recv(1.0);
  deadband.recv(0.0);
  deadband.recv(0.0);
}

TEST(Deadband, ForwardsWhenReachingMaxForTheFirstTimeEvenWhenWithinDeadband)
{
  MockSink<float> sink;
  EXPECT_CALL(sink, recv(99.0f)).Times(1);
  EXPECT_CALL(sink, recv(100.0f)).Times(1);

  channel::Deadband deadband(sink, 0.0f, 100.0f, 2.0f);

  deadband.recv(99.0);
  deadband.recv(100.0);
  deadband.recv(100.0);
}
