#ifndef LUMADI_DETAIL_LOGGER_ILOGHANDLER_H
#define LUMADI_DETAIL_LOGGER_ILOGHANDLER_H

#include <vector>
#include <string_view>
#include "log.h"

namespace Lumadi
{
  class LogHandlerBase
  {
  public:
    virtual ~LogHandlerBase() = default;
    void Handle(std::vector<LogData> messages)
    {
      for (const auto &message : messages)
        Handle(message);
    };
    virtual void Handle(const LogData &logData) = 0;
  };
}

#endif // LUMADI_DETAIL_LOGGER_ILOGHANDLER_H