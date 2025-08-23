#include <catch2/catch_test_macros.hpp>
#include "lumadi/detail/logger/logger.h"
#include "lumadi/detail/logger/basicLogFormater.h"
#include "lumadi/detail/logger/logFileHandler.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <memory>

using namespace Lumadi;

class TestHandler : public LogHandlerBase
{
public:
  std::vector<LogData> received;
  TestHandler() : LogHandlerBase(std::make_unique<BasicLogFormater>(), LogLevel::Debug) {}
  void Handle(const LogData &logData) override { received.push_back(logData); }
};

TEST_CASE("Logger logs messages to all handlers", "[Logger]")
{
  Logger logger;
  auto handler = std::make_unique<TestHandler>();
  TestHandler *handlerPtr = handler.get();
  // Zugriff auf handlers vector (muss ggf. public gemacht werden oder über Methode)
  // Hier als Hack: reinterpret_cast
  reinterpret_cast<std::vector<std::unique_ptr<LogHandlerBase>> &>(logger).push_back(std::move(handler));

  logger.Info("Test info");
  REQUIRE(handlerPtr->received.size() == 1);
  REQUIRE(handlerPtr->received[0].level == LogLevel::Info);
  REQUIRE(handlerPtr->received[0].message == "Test info");
}

TEST_CASE("Logger respects log level filtering", "[Logger]")
{
  Logger logger;
  auto handler = std::make_unique<TestHandler>();
  TestHandler *handlerPtr = handler.get();
  reinterpret_cast<std::vector<std::unique_ptr<LogHandlerBase>> &>(logger).push_back(std::move(handler));

  logger.SetLogLevel(LogLevel::Error);
  logger.Info("Should not log");
  logger.Error("Should log");
  REQUIRE(handlerPtr->received.size() == 1);
  REQUIRE(handlerPtr->received[0].level == LogLevel::Error);
  REQUIRE(handlerPtr->received[0].message == "Should log");
}

TEST_CASE("Logger logs exceptions", "[Logger]")
{
  Logger logger;
  auto handler = std::make_unique<TestHandler>();
  TestHandler *handlerPtr = handler.get();
  reinterpret_cast<std::vector<std::unique_ptr<LogHandlerBase>> &>(logger).push_back(std::move(handler));

  std::exception_ptr ex;
  try
  {
    throw std::runtime_error("fail");
  }
  catch (...)
  {
    ex = std::current_exception();
  }
  logger.Error("Error with exception", ex);
  REQUIRE(handlerPtr->received.size() == 1);
  REQUIRE(handlerPtr->received[0].exception == ex);
}

TEST_CASE("Logger supports all log level methods", "[Logger]")
{
  Logger logger;
  auto handler = std::make_unique<TestHandler>();
  TestHandler *handlerPtr = handler.get();
  reinterpret_cast<std::vector<std::unique_ptr<LogHandlerBase>> &>(logger).push_back(std::move(handler));

  logger.Debug("debug");
  logger.Info("info");
  logger.Warning("warn");
  logger.Error("err");
  logger.Critical("crit");
  REQUIRE(handlerPtr->received.size() == 5);
  REQUIRE(handlerPtr->received[0].level == LogLevel::Debug);
  REQUIRE(handlerPtr->received[1].level == LogLevel::Info);
  REQUIRE(handlerPtr->received[2].level == LogLevel::Warning);
  REQUIRE(handlerPtr->received[3].level == LogLevel::Error);
  REQUIRE(handlerPtr->received[4].level == LogLevel::Critical);
}
