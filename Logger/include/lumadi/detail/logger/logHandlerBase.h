#ifndef LUMADI_DETAIL_LOGGER_ILOGHANDLER_H
#define LUMADI_DETAIL_LOGGER_ILOGHANDLER_H

#include <vector>
#include <string_view>
#include "log.h"
#include "ILogFormater.h"

namespace Lumadi
{
  class LogHandlerBase
  {
  public:
    LogHandlerBase(std::unique_ptr<ILogFormater> logFormater, LogLevel level) : logFormater_(std::move(logFormater)), currentLogLevel_(level) {}
    virtual ~LogHandlerBase() = default;
    void Handle(std::vector<LogData> &messages);
    virtual void Handle(const LogData &logData) = 0;
    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel() const;

  protected:
    LogLevel currentLogLevel_;
    std::unique_ptr<ILogFormater> logFormater_;
  };
}

#endif // LUMADI_DETAIL_LOGGER_ILOGHANDLER_H