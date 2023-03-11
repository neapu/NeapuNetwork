#include "TcpServer.h"
#include "ModuleLoggerNetwork.h"
#include <WinSock2.h>
#include <memory>
#include <winbase.h>

using namespace neapu;

bool TcpServer::Init(int _port, std::string _ipAddr, IP_TYPE _ipType)
{
    m_listemSocket = TcpSocket::MakeTcpSocket();

    bool ret = m_listemSocket->Init(_port, _ipAddr, _ipType);
    if (!ret) {
        LOG_ERROR << "Failed to init Listen Socket";
        return ret;
    }

    m_listemSocket->SetNonBlock(true);

    SocketEvent::SetSocket(m_listemSocket);

    return ret;
}

bool TcpServer::Listen()
{
    if (!m_listemSocket) {
        return false;
    }
    int ret = m_listemSocket->Listen();
    if (ret < 0) {
        LOG_ERROR << "Failed to listen";
        return false;
    }
    return true;
}

void TcpServer::OnClientRecv(RecvDataCallback _cb)
{
    m_clientRecvCallback = _cb;
}

void TcpServer::OnClientClose(RecvDataCallback _cb)
{
    m_clientCloseCallback = _cb;
}

void TcpServer::OnAccept(RecvDataCallback _cb)
{
    m_acceptCallback = _cb;
}

void TcpServer::OnClientRecv(BaseSocketPtr _sock)
{
    if (m_clientRecvCallback) {
        m_clientRecvCallback(_sock);
    }
}

void TcpServer::OnClientClose(BaseSocketPtr _sock)
{
    if (m_clientCloseCallback) {
        m_clientCloseCallback(_sock);
    }
}

void TcpServer::OnAccept(BaseSocketPtr _sock)
{
    if (m_acceptCallback) {
        m_acceptCallback(_sock);
    }
}

void TcpServer::OnRecvData(BaseSocketPtr _sock)
{
    if (_sock == m_listemSocket) {
        auto client = m_listemSocket->Accept();
        if (client) {
            OnAccept(client);
            client->SetNonBlock(true);
            SetSocket(client);
        }
    } else {
        OnClientRecv(_sock);
    }
}

void TcpServer::OnClose(BaseSocketPtr _sock)
{
    if(_sock!=m_listemSocket){
        OnClientClose(_sock);
    }
}