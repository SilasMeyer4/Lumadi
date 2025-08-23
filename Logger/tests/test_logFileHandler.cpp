#include <catch2/catch_test_macros.hpp>
#include "lumadi/detail/logger/logFileHandler.h"
#include "lumadi/detail/logger/basicLogFormater.h"
#include "lumadi/detail/logger/log.h"
#include <fstream>
#include <cstdio>
#include <string>

using namespace Lumadi;

TEST_CASE("LogFileHandler writes formatted log to file", "[LogFileHandler]")
{
  const std::string filename = "test_logfilehandler.log";
  std::remove(filename.c_str());

  auto formater = std::make_unique<BasicLogFormater>();
  LogFileHandler handler(std::move(formater), filename, LogLevel::Info);

  LogData data{LogLevel::Info, "Test log entry", std::chrono::system_clock::from_time_t(1724341234), nullptr};
  handler.Handle(data);

  std::ifstream in(filename);
  REQUIRE(in.is_open());
  std::string line;
  std::getline(in, line);
  REQUIRE(line.find("Test log entry") != std::string::npos);
  REQUIRE(line.find("[Info]") != std::string::npos);
  in.close();
  std::remove(filename.c_str());
}

TEST_CASE("LogFileHandler closes file on destruction", "[LogFileHandler]")
{
  const std::string filename = "test_logfilehandler_close.log";
  std::remove(filename.c_str());
  {
    auto formater = std::make_unique<BasicLogFormater>();
    LogFileHandler handler(std::move(formater), filename, LogLevel::Info);
    LogData data{LogLevel::Info, "Entry", std::chrono::system_clock::now(), nullptr};
    handler.Handle(data);
  }
  std::ifstream in(filename);
  REQUIRE(!in.is_open());
  in.close();
  std::remove(filename.c_str());
}
