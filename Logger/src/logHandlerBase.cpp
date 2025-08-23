
#include "./include/lumadi/detail/logger/logHandlerBase.h"

void Lumadi::LogHandlerBase::Handle(std::vector<LogData> &messages)
{
  for (const auto &message : messages)
    Handle(message);
}

void Lumadi::LogHandlerBase::SetLogLevel(LogLevel level)
{
  currentLogLevel_ = level;
}

Lumadi::LogLevel Lumadi::LogHandlerBase::GetLogLevel() const
{
  return currentLogLevel_;
}
