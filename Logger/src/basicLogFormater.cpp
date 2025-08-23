#include "./include/lumadi/detail/logger/basicLogFormater.h"
#include <chrono>
#include <exception>
#include "./include/lumadi/detail/logger/log.h"

std::string Lumadi::BasicLogFormater::Format(const LogData &logData)
{
  std::string exceptionMsg{};
  std::string levelName = GetLogLevelName(logData.level);
  if (logData.exception)
  {
    try
    {
      std::rethrow_exception(logData.exception);
    }
    catch (const std::exception &ex)
    {
      exceptionMsg = " Exception: " + std::string(ex.what());
    }
    catch (...)
    {
      exceptionMsg = " Exception: unknown";
    }
  }
  return "[" + std::to_string(std::chrono::system_clock::to_time_t(logData.timestamp)) + "] " +
         "[" + levelName + "] " +
         logData.message.data() +
         exceptionMsg;
}