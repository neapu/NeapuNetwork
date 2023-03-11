#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include <memory>
#include <string>
#include "NetworkDef.h"
#include "BaseSocket.h"

namespace neapu {

class NEAPU_NETWORK_EXPORT UdpSocket : public BaseSocket {
public:
    bool Init(int _port, std::string _ipAddr = std::string(), IP_TYPE _ipType = IP_TYPE::IPv4);
    int WaitReceiveData(int _timeout);
    int SendData(void* _data, size_t _len, std::string _targetAddr, int _targetPort);
    int RecvData(void* _buffer, size_t _bufferLen, std::string& _targetAddr, int& _targetPort);

    static std::shared_ptr<UdpSocket> MakeUdpSocket();
};

using UdpSocketPtr = std::shared_ptr<UdpSocket>;
} // namespace neapu

#endif // __UDPSOCKET_H__