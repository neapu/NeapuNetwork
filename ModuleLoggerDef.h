#ifndef __MODULELOGGERDEF_H__
#define __MODULELOGGERDEF_H__

#include <functional>

namespace neapu{
// log level
constexpr int LOG_LEVEL_ERROR = 1;
constexpr int LOG_LEVEL_INFO = 2;
constexpr int LOG_LEVEL_DEBUG = 3;

// log callback
using LogCallback = std::function<void(int _level, const char* msg)>;
}

#endif // __MODULELOGGERDEF_H__