#ifndef LUMADI_DETAIL_LOGGER_LOG_H
#define LUMADI_DETAIL_LOGGER_LOG_H

#include "logger.h"
#include <chrono>
#include <exception>

struct LogData
{
  Lumadi::LogLevel level;
  std::string_view message;
  std::chrono::system_clock::time_point timestamp;
  std::exception_ptr exception;
};

#endif // LUMADI_DETAIL_LOGGER_LOG_H