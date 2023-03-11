#include "TcpSocket.h"
#include <memory>
#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2ipdef.h>
#include <ws2tcpip.h>
#else
#endif
#include "ModuleLoggerNetwork.h"

using namespace neapu;

bool TcpSocket::Init(int _port, std::string _ipAddr, IP_TYPE _ipType)
{
    return BaseSocket::Init(_port, _ipAddr, _ipType, true);
}

int TcpSocket::Connect(int _port, std::string _ipAddr)
{
    if (m_fd <= 0) return -1;
    if (m_ipType == IP_TYPE::IPv4) {
        sockaddr_in sin = {0};
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);
        int ret = inet_pton(AF_INET, _ipAddr.c_str(), &sin.sin_addr.s_addr);
        if (ret < 0) {
            LOG_ERROR << "Failed to parse the IP address";
            return ret;
        }
        return ::connect(m_fd, (sockaddr*)&sin, sizeof(sin));
    } else if (m_ipType == IP_TYPE::IPv6) {
        sockaddr_in6 sin = {0};
        sin.sin6_family = AF_INET6;
        sin.sin6_port = htons(_port);
        int ret = inet_pton(AF_INET6, _ipAddr.c_str(), &sin.sin6_addr);
        if (ret < 0) {
            LOG_ERROR << "Failed to parse the IP address";
            return ret;
        }
        return ::connect(m_fd, (sockaddr*)&sin, sizeof(sin));
    }
    return -1;
}

int TcpSocket::Listen()
{
    if(m_fd<=0){
        return -1;
    }
    return ::listen(m_fd, 10);
}

std::shared_ptr<TcpSocket> TcpSocket::Accept()
{
    sockaddr_storage sin;
    int len = sizeof(sin);
    SOCKET_FD accpet_fd = ::accept(m_fd, (sockaddr*)&sin, &len);
    if(accpet_fd<=0){
        return nullptr;
    }
    auto rst = MakeTcpSocket();
    rst->m_fd = accpet_fd;
    rst->SetRemoteInfo(&sin);
    return rst;
}

int TcpSocket::Send(const void* _data, size_t _len)
{
    if (m_fd <= 0) return -1;
    return ::send(m_fd, (const char*)_data, _len, 0);
}

int TcpSocket::Recv(void* _data, size_t _len)
{
    if (m_fd <= 0) return -1;
    return ::recv(m_fd, (char*)_data, _len, 0);
}

std::shared_ptr<TcpSocket> TcpSocket::MakeTcpSocket()
{
    return std::make_shared<TcpSocket>();
}

void TcpSocket::SetRemoteInfo(struct sockaddr_storage* p_sin)
{
    char buffer[128] = {0};
    if(p_sin->ss_family == AF_INET){
        sockaddr_in* p = (sockaddr_in*)p_sin;
        inet_ntop(AF_INET, &p->sin_addr.s_addr, buffer, 128);
        m_remoteAddr = buffer;
        m_remotePort = ntohs(p->sin_port);
    } else if(p_sin->ss_family == AF_INET6){
        sockaddr_in6* p = (sockaddr_in6*)p_sin;
        inet_ntop(AF_INET, &p->sin6_addr, buffer, 128);
        m_remoteAddr = buffer;
        m_remotePort = ntohs(p->sin6_port);
    }
}
