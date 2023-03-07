// 不要在头文件里包含
#ifndef __MODULELOGGER_NETWORK_H__
#define __MODULELOGGER_NETWORK_H__

#include "ModuleLogger.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__))

#define LOG_ERROR neapu::ModuleLogger(neapu::LOG_LEVEL_ERROR) << "[" << __FILENAME__ << ":" << __LINE__ << "][" << __FUNCTION__ << "]"
#define LOG_INFO neapu::ModuleLogger(neapu::LOG_LEVEL_INFO) << "[" << __FILENAME__ << ":" << __LINE__ << "][" << __FUNCTION__ << "]"
#define LOG_DEBUG neapu::ModuleLogger(neapu::LOG_LEVEL_DEBUG) << "[" << __FILENAME__ << ":" << __LINE__ << "][" << __FUNCTION__ << "]"

#endif // __MODULELOGGER_NETWORK_H__