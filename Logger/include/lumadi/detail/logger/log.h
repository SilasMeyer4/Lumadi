#ifndef LUMADI_DETAIL_LOGGER_LOG_H
#define LUMADI_DETAIL_LOGGER_LOG_H

#include "logger.h"
#include <chrono>
#include <exception>

namespace Lumadi
{
  enum class LogLevel
  {
    Debug,
    Info,
    Warning,
    Error,
    Critical
  };

  struct LogData
  {
    Lumadi::LogLevel level;
    std::string_view message;
    std::chrono::system_clock::time_point timestamp;
    std::exception_ptr exception;
  };

  std::string GetLogLevelName(Lumadi::LogLevel level)
  {
    switch (level)
    {
    case Lumadi::LogLevel::Debug:
      return "Debug";
    case Lumadi::LogLevel::Info:
      return "Info";
    case Lumadi::LogLevel::Warning:
      return "Warning";
    case Lumadi::LogLevel::Error:
      return "Error";
    case Lumadi::LogLevel::Critical:
      return "Critical";
    default:
      return "Unknown";
    }
  }
}

#endif // LUMADI_DETAIL_LOGGER_LOG_H