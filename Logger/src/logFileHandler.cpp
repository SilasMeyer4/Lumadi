#include "./include/lumadi/detail/logger/logFileHandler.h"

Lumadi::LogFileHandler::~LogFileHandler()
{
  if (logFile_.is_open())
  {
    logFile_.close();
  }
}

void Lumadi::LogFileHandler::Handle(const LogData &logData)
{
  logFile_ << logFormater_->Format(logData);
}
