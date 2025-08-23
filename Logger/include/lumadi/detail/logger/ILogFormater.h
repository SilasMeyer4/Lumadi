#ifndef LUMADI_DETAIL_LOGGER_ILOGFORMATER_H
#define LUMADI_DETAIL_LOGGER_ILOGFORMATER_H

#include "log.h"

namespace Lumadi
{
  class ILogFormater
  {
  public:
    virtual ~ILogFormater() = default;
    virtual std::string Format(const LogData &logData) = 0;
  };
}

#endif // LUMADI_DETAIL_LOGGER_ILOGFORMATER_H