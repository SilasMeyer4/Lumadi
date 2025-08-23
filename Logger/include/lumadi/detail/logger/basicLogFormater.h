

#ifndef LUMADI_DETAIL_LOGGER_BASICLOGFORMATER_H
#define LUMADI_DETAIL_LOGGER_BASICLOGFORMATER_H

#include "ILogFormater.h"

namespace Lumadi
{
  class BasicLogFormater : public ILogFormater
  {
  public:
    std::string Format(const LogData &logData) override;
  };
}

#endif // LUMADI_DETAIL_LOGGER_BASICLOGFORMATER_H