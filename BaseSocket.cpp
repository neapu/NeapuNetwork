#include "BaseSocket.h"
#include "ModuleLoggerNetwork.h"
#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2ipdef.h>
#include <ws2tcpip.h>
#else
#endif

using namespace neapu;

bool BaseSocket::Init(int _port, std::string _ipAddr, IP_TYPE _ipType, bool isUdp)
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
#endif
    sockaddr* p_sin = nullptr;
    size_t sin_len = 0;
    int ret = 0;

    if (_ipType == IP_TYPE::IPv4) {
        if (isUdp) {
            m_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        } else {
            m_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        }

        if (m_fd <= 0) {
            LOG_ERROR << "Failed to create socket." << GetErrorCode();
            return false;
        }

        sockaddr_in sin = {0};
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_port);
        if (_ipAddr.empty()) {
            sin.sin_addr.s_addr = INADDR_ANY;
        } else {
            ret = inet_pton(AF_INET, _ipAddr.c_str(), &sin.sin_addr.s_addr);
            if (ret < 0) {
                LOG_ERROR << "Failed to convert ip address[" << _ipAddr << "]." << GetErrorCode();
                return false;
            }
        }
        p_sin = (sockaddr*)&sin;
        sin_len = sizeof(sin);
    } else if (_ipType == IP_TYPE::IPv6) {
        if (isUdp) {
            m_fd = ::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        } else {
            m_fd = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        }
        if (m_fd <= 0) {
            LOG_ERROR << "Failed to create socket." << GetErrorCode();
            return false;
        }

        sockaddr_in6 sin = {0};
        sin.sin6_family = AF_INET6;
        sin.sin6_port = htons(_port);
        if (_ipAddr.empty()) {
            sin.sin6_addr = in6addr_any;
        } else {
            ret = inet_pton(AF_INET, _ipAddr.c_str(), &sin.sin6_addr);
            if (ret < 0) {
                LOG_ERROR << "Failed to convert ip address[" << _ipAddr << "]." << GetErrorCode();
                return false;
            }
        }
        p_sin = (sockaddr*)&sin;
        sin_len = sizeof(sin);
    } else {
        LOG_ERROR << "Unknow IP Type[" << (int)_ipType << "]";
        return false;
    }

    LOG_INFO << "UdpSocket initialization completed.FD:[" << m_fd << "]";
    return true;
}

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

int neapu::BaseSocket::SetNonBlock(bool nonblock)
{
#ifdef _WIN32
    unsigned long nonblocking = (unsigned long)nonblock;
    if (ioctlsocket(m_fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
        int err = GetErrorCode();
        LOG_INFO << "ioctlsocket ERROR:" << err;
        return -1;
    }
#else
    int flags;
    if ((flags = fcntl(m_fd, F_GETFL, NULL)) < 0) {
        LOG_INFO << "fcntl(fd, F_GETFL) ERROR";
        return -1;
    }
    if (nonblock) {
        if (fcntl(m_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            LOG_INFO << "fcntl(fd, F_GETFL) ERROR";
            return -1;
        }
    } else if (nonblock) {
        if (fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
            LOG_INFO << "fcntl(fd, F_GETFL) ERROR";
            return -1;
        }
    }

#endif
    m_nonBlock = nonblock;
    return 0;
}