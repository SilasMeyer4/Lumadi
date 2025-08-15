#include "logger.h"

void Lumadi::Logger::Log(LogLevel logLevel, std::string_view message)
{
  Log(logLevel, message, nullptr);
}

void Lumadi::Logger::Log(LogLevel logLevel, std::string_view message, std::exception_ptr exception)
{
  if (logLevel < this->logLevel)
    return; // Skip logging if the log level is lower than the current logger's level

  LogData logData{logLevel, message, std::chrono::system_clock::now(), exception};

  for (const auto &handler : handlers)
  {
    handler->Handle(logData);
  }
}

void Lumadi::Logger::Info(std::string_view message)
{
  Info(message, nullptr);
}

void Lumadi::Logger::Info(std::string_view message, std::exception_ptr exception)
{
  Log(LogLevel::Info, message, exception);
}

void Lumadi::Logger::Error(std::string_view message)
{
  Error(message, nullptr);
}

void Lumadi::Logger::Error(std::string_view message, std::exception_ptr exception)
{
  Log(LogLevel::Error, message, exception);
}

void Lumadi::Logger::Debug(std::string_view message)
{
  Debug(message, nullptr);
}

void Lumadi::Logger::Debug(std::string_view message, std::exception_ptr exception)
{
  Log(LogLevel::Debug, message, exception);
}

void Lumadi::Logger::Warning(std::string_view message)
{
  Warning(message, nullptr);
}

void Lumadi::Logger::Warning(std::string_view message, std::exception_ptr exception)
{
  Log(LogLevel::Warning, message, exception);
}

void Lumadi::Logger::Critical(std::string_view message)
{
  Critical(message, nullptr);
}

void Lumadi::Logger::Critical(std::string_view message, std::exception_ptr exception)
{
  Log(LogLevel::Critical, message, exception);
}

void Lumadi::Logger::SetLogLevel(LogLevel level)
{
  logLevel = level;
}

Lumadi::LogLevel Lumadi::Logger::GetLogLevel() const
{
  return logLevel;
}
