/*
此类不导出
不要在头文件里包含
*/
#ifndef __MODULELOGGER_H__
#define __MODULELOGGER_H__

#include "ModuleLoggerDef.h"
#include <sstream>
#include <string>

namespace neapu {
class ModuleLogger {
public:
    static void SetCallback(LogCallback _cb);
    ModuleLogger(int _level) : m_level(_level) {}
    ~ModuleLogger()
    {
        if (m_callback) {
            m_callback(m_level, m_data.str().c_str());
        }
    }

    template <class T>
    ModuleLogger& operator<<(T&& t)
    {
        m_data << std::forward<T>(t);
        return *this;
    }

private:
    static LogCallback m_callback;
    int m_level;
    std::stringstream m_data;
};
} // namespace neapu

#endif // __MODULELOGGER_H__