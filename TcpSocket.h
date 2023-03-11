#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include <memory>
#include <string>
#include "BaseSocket.h"
struct sockaddr_storage;
namespace neapu {
class NEAPU_NETWORK_EXPORT TcpSocket : public BaseSocket {
public:
    bool Init(int _port = 0, std::string _ipAddr = std::string(), IP_TYPE _ipType = IP_TYPE::IPv4);
    int Connect(int _port, std::string _ipAddr = std::string());
    int Listen();
    std::shared_ptr<TcpSocket> Accept();

    int Send(const void* _data, size_t _len);
    int Recv(void* _data, size_t _len);

    static std::shared_ptr<TcpSocket> MakeTcpSocket();
protected:
    void SetRemoteInfo(struct sockaddr_storage* p_sin);

protected:
    std::string m_remoteAddr;
    int m_remotePort = 0;
};
using TcpSocketPtr = std::shared_ptr<TcpSocket>;
}

#endif // __TCPSOCKET_H__