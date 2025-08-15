#ifndef LUMADI_DETAIL_LOGGER_ILOGGER_H
#define LUMADI_DETAIL_LOGGER_ILOGGER_H

#include <string_view>

namespace Lumadi
{
  class ILogger
  {
  public:
    virtual ~ILogger() = default;

    virtual void Log(std::string_view message) = 0;
  };
}
#endif // LUMADI_DETAIL_LOGGER_ILOGGER_H