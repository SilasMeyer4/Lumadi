#ifndef LUMADI_DETAIL_LOGGER_LOGFILEHANDLER_H
#define LUMADI_DETAIL_LOGGER_LOGFILEHANDLER_H

#include "logHandlerBase.h"
#include <fstream>
#include "log.h"

namespace Lumadi
{
  class LogFileHandler : public LogHandlerBase
  {
  public:
    LogFileHandler(std::unique_ptr<ILogFormater> logFormater, const std::string &filePath, LogLevel level)
        : LogHandlerBase(std::move(logFormater), level), logFile_(filePath.c_str(), std::ios::app) {}
    ~LogFileHandler();
    void Handle(const LogData &logData) override;

  private:
    std::ofstream logFile_;
  };
}

#endif // LUMADI_DETAIL_LOGGER_LOGFILEHANDLER_H