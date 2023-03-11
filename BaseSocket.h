#ifndef __BASESOCKET_H__
#define __BASESOCKET_H__

#include "NetworkDef.h"
#include "ModuleLoggerDef.h"
#include <memory>
#include <string>

namespace neapu {
class NEAPU_NETWORK_EXPORT BaseSocket {
public:
    void SetLogCallback(LogCallback _cb);
    int SetNonBlock(bool nonblock);
    bool IsNonBlock() { return m_nonBlock; }
    SOCKET_FD GetFd() { return m_fd; }

protected:
    bool Init(int _port, std::string _ipAddr = std::string(), IP_TYPE _ipType = IP_TYPE::IPv4,
              bool isUdp = false);
    static int GetErrorCode();

protected:
    SOCKET_FD m_fd = INVALID_SOCKET;
    IP_TYPE m_ipType = IP_TYPE::IPv4;
    bool m_nonBlock = false;

    LogCallback m_logCallback;
};

using BaseSocketPtr = std::shared_ptr<BaseSocket>;
} // namespace neapu

#endif // __BASESOCKET_H__