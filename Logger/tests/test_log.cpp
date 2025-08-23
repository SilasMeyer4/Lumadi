#include <catch2/catch_test_macros.hpp>
#include "lumadi/detail/logger/log.h"
#include <chrono>
#include <string>

using namespace Lumadi;

TEST_CASE("GetLogLevelName returns correct string for each LogLevel", "[LogLevel]")
{
  REQUIRE(GetLogLevelName(LogLevel::Debug) == "Debug");
  REQUIRE(GetLogLevelName(LogLevel::Info) == "Info");
  REQUIRE(GetLogLevelName(LogLevel::Warning) == "Warning");
  REQUIRE(GetLogLevelName(LogLevel::Error) == "Error");
  REQUIRE(GetLogLevelName(LogLevel::Critical) == "Critical");
}

TEST_CASE("LogData stores and exposes its members correctly", "[LogData]")
{
  auto now = std::chrono::system_clock::now();
  std::string msg = "Test message";
  LogData data{LogLevel::Warning, msg, now, nullptr};
  REQUIRE(data.level == LogLevel::Warning);
  REQUIRE(data.message == msg);
  REQUIRE(data.timestamp == now);
  REQUIRE(data.exception == nullptr);
}
