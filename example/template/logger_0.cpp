#include "logger_0.h"

namespace Logging {
Logger& Logger::Inst()
{
    static Logger inst;

    return inst;
}

std::string Logger::Get() const { return oss_.str(); }

void Logger::Clear()
{
    oss_.str("");
    oss_.clear(std::stringstream::goodbit);
}
}  // namespace Logging
