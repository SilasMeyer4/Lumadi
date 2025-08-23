
#ifndef LUMADI_DETAIL_LOGGER_LOGGER_H
#define LUMADI_DETAIL_LOGGER_LOGGER_H

#include "ILogger.h"
#include "logHandlerBase.h"
#include <memory>
#include <vector>

namespace Lumadi
{

  class Logger
  {
  private:
    LogLevel logLevel = LogLevel::Info;
    std::vector<std::unique_ptr<LogHandlerBase>> handlers;

  public:
    /**
     * @brief Logs a message to the console.
     * @param message The message to log.
     */
    void Log(LogLevel logLevel, std::string_view message);
    void Log(LogLevel logLevel, std::string_view message, std::exception_ptr exception);

    void Info(std::string_view message);
    void Info(std::string_view message, std::exception_ptr exception);
    void Error(std::string_view message);
    void Error(std::string_view message, std::exception_ptr exception);
    void Debug(std::string_view message);
    void Debug(std::string_view message, std::exception_ptr exception);
    void Warning(std::string_view message);
    void Warning(std::string_view message, std::exception_ptr exception);
    void Critical(std::string_view message);
    void Critical(std::string_view message, std::exception_ptr exception);

    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel() const;
  };
}

#endif // LUMADI_DETAIL_LOGGER_LOGGER_H