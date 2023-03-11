#include "SocketEvent.h"
#include <WinSock2.h>
#include <mutex>

using namespace neapu;

void SocketEvent::OnRecvData(RecvDataCallback _cb)
{
    m_recvCallback = _cb;
}

void SocketEvent::OnClose(RecvDataCallback _cb)
{
    m_closeCallback = _cb;
}

int neapu::SocketEvent::SetSocket(BaseSocketPtr _sock)
{
    std::unique_lock<std::mutex> locker(m_socketListMutex);
    m_socketList[_sock->GetFd()] = _sock;
    return 0;
}

int SocketEvent::Loop()
{
    pollfd* pfds = nullptr;
    m_stopLoop = false;
    int index = 0;
    while (!m_stopLoop) {
        {
            std::unique_lock<std::mutex> locker(m_socketListMutex);
            int fd_count = m_socketList.size();
            pfds = new pollfd[fd_count];
            index = 0;
            for (auto& item : m_socketList) {
                pfds[index].fd = item.first;
                pfds[index].events = POLLIN;
                pfds[index].revents = 0;
                ++index;
            }
        }
        int ret = WSAPoll(pfds, index, 1);
        if (ret < 0) {
            delete[] pfds;
            return ret;
        }
        if (ret == 0) {
            delete[] pfds;
            continue;
        }

        for (int i = 0; i < index; i++) {
            if (pfds->revents & POLLIN) {
                std::unique_lock<std::mutex> locker(m_socketListMutex);
                OnRecvData(m_socketList[pfds->fd]);
            } else if (pfds->revents & (POLLERR | POLLHUP)) {
                std::unique_lock<std::mutex> locker(m_socketListMutex);
                OnClose(m_socketList[pfds->fd]);
            }
        }
    }
    return 0;
}

void SocketEvent::OnRecvData(BaseSocketPtr _sock)
{
    if (m_recvCallback) {
        m_recvCallback(_sock);
    }
}

void SocketEvent::OnClose(BaseSocketPtr _sock)
{
    if (m_closeCallback) {
        m_closeCallback(_sock);
    }
    auto key = _sock->GetFd();
    m_socketList.erase(key);
}