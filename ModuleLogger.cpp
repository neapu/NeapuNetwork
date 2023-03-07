#include "ModuleLogger.h"

neapu::LogCallback neapu::ModuleLogger::m_callback;

void neapu::ModuleLogger::SetCallback(LogCallback _cb)
{
    m_callback = _cb;
}
