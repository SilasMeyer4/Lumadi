#include <catch2/catch_test_macros.hpp>
#include "lumadi/detail/logger/basicLogFormater.h"
#include "lumadi/detail/logger/log.h"
#include <chrono>
#include <string>
#include <exception>

using namespace Lumadi;

TEST_CASE("BasicLogFormater formats log message without exception", "[BasicLogFormater]")
{
  BasicLogFormater formatter;
  LogData data{LogLevel::Info, "Hello World", std::chrono::system_clock::from_time_t(1724341234), nullptr};
  std::string result = formatter.Format(data);
  REQUIRE(result.find("1724341234") != std::string::npos);
  REQUIRE(result.find("Info") != std::string::npos);
  REQUIRE(result.find("Hello World") != std::string::npos);
  REQUIRE(result.find("Exception") == std::string::npos);
}

TEST_CASE("BasicLogFormater formats log message with std::exception", "[BasicLogFormater]")
{
  BasicLogFormater formatter;
  std::exception_ptr ex;
  try
  {
    throw std::runtime_error("fail!");
  }
  catch (...)
  {
    ex = std::current_exception();
  }
  LogData data{LogLevel::Error, "Something happened", std::chrono::system_clock::from_time_t(1724341234), ex};
  std::string result = formatter.Format(data);
  REQUIRE(result.find("1724341234") != std::string::npos);
  REQUIRE(result.find("Error") != std::string::npos);
  REQUIRE(result.find("Something happened") != std::string::npos);
  REQUIRE(result.find("Exception: fail!") != std::string::npos);
}
