#include "BaseSocket.h"
#include "ModuleLogger.h"
#ifdef _WIN32
#include <WinSock2.h>
#else
#endif

using namespace neapu;

void neapu::BaseSocket::SetLogCallback(LogCallback _cb)
{
    m_logCallback = _cb;
    ModuleLogger::SetCallback(_cb);
}

int neapu::BaseSocket::GetErrorCode()
{
#ifdef _WIN32
    return WSAGetLastError();
#else
#endif
}
