#include "UdpSocket.h"
#include <memory>
#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2ipdef.h>
#include <ws2tcpip.h>
#else
#endif
#include "ModuleLoggerNetwork.h"

using namespace neapu;

bool neapu::UdpSocket::Init(int _port, std::string _ipAddr, IP_TYPE _ipType)
{
    return BaseSocket::Init(_port, _ipAddr, _ipType, true);
}

int neapu::UdpSocket::WaitReceiveData(int _timeout)
{
    if (m_fd <= 0) {
        LOG_ERROR << "m_fd is Invalid." << m_fd;
        return -1;
    }

    if (m_nonBlock == false) {
        LOG_ERROR << "The socket is blocking.";
        return -1;
    }

#ifdef _WIN32
    int ret = 0;
    WSAPOLLFD pfd;
    pfd.events = POLLIN;
    pfd.fd = m_fd;
    pfd.revents = 0;

    ret = WSAPoll(&pfd, 1, _timeout);
    if (ret <= 0) {
        return ret;
    }

    if (pfd.revents & POLLIN) {
        return 1;
    }
    return 0;
#else
#endif
}

int UdpSocket::SendData(void* _data, size_t _len, std::string _targetAddr, int _targetPort)
{
    if (m_ipType == IP_TYPE::IPv4) {
        sockaddr_in target = {0};
        target.sin_family = AF_INET;
        target.sin_port = htons(_targetPort);
        int ret = inet_pton(AF_INET, _targetAddr.c_str(), &target.sin_addr.s_addr);
        if (ret < 0) {
            LOG_ERROR << "Failed to convert ip address[" << _targetAddr << "]." << GetErrorCode();
            return -1;
        }
        return ::sendto(m_fd, (const char*)_data, _len, 0, (sockaddr*)&target, sizeof(target));
    } else if (m_ipType == IP_TYPE::IPv6) {
        sockaddr_in6 target = {0};
        target.sin6_family = AF_INET6;
        target.sin6_port = htons(_targetPort);
        int ret = inet_pton(AF_INET6, _targetAddr.c_str(), &target.sin6_addr);
        if (ret < 0) {
            LOG_ERROR << "Failed to convert ip address[" << _targetAddr << "]." << GetErrorCode();
            return -1;
        }
        return ::sendto(m_fd, (const char*)_data, _len, 0, (sockaddr*)&target, sizeof(target));
    }
    return -1;
}

int UdpSocket::RecvData(void* _buffer, size_t _bufferLen, std::string& _targetAddr,
                        int& _targetPort)
{
    sockaddr_storage sin;
    int addrLen = sizeof(sockaddr_storage);
    int ret = ::recvfrom(m_fd, (char*)_buffer, _bufferLen, 0, (sockaddr*)&sin, &addrLen);
    if (ret <= 0) {
        return ret;
    }
    constexpr int address_string_len = 64;
    char address_string[address_string_len];
    if (sin.ss_family == AF_INET) {
        inet_ntop(AF_INET, &(((sockaddr_in*)&sin)->sin_addr.s_addr), address_string,
                  address_string_len);
        _targetPort = ntohs(((sockaddr_in*)&sin)->sin_port);
    } else if (sin.ss_family == AF_INET6) {
        inet_ntop(AF_INET6, &(((sockaddr_in6*)&sin)->sin6_addr), address_string,
                  address_string_len);
        _targetPort = ntohs(((sockaddr_in6*)&sin)->sin6_port);
    }
    _targetAddr = address_string;
    return ret;
}

std::shared_ptr<UdpSocket> UdpSocket::MakeUdpSocket()
{
    return std::make_shared<UdpSocket>();
}
