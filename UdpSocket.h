#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include <string>
#include "NetworkDef.h"
#include "BaseSocket.h"

namespace neapu {

class UdpSocket : public BaseSocket {
public:
    bool Init(int _port, std::string _ipAddr = std::string(), IP_TYPE _ipType = IP_TYPE::IPv4);

    int SetNonBlock(bool nonblock);

    int WaitReceiveData(int _timeout);

    int SendData(void* _data, size_t _len, std::string _targetAddr, int _targetPort);

    int RecvData(void* _buffer, size_t _bufferLen, std::string& _targetAddr, int& _targetPort);

protected:
    SOCKET_FD m_fd = INVALID_SOCKET;
    IP_TYPE m_ipType = IP_TYPE::IPv4;
    bool m_nonBlock = false;
};
} // namespace neapu

#endif // __UDPSOCKET_H__