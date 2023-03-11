#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "SocketEvent.h"
#include "TcpSocket.h"

namespace neapu {
class NEAPU_NETWORK_EXPORT TcpServer : public SocketEvent {
public:
    bool Init(int _port, std::string _ipAddr = std::string(), IP_TYPE _ipType = IP_TYPE::IPv4);
    bool Listen();

    void OnClientRecv(RecvDataCallback _cb);
    void OnClientClose(RecvDataCallback _cb);
    void OnAccept(RecvDataCallback _cb);

    void OnRecvData(RecvDataCallback _cb) = delete;
    void OnClose(RecvDataCallback _cb) = delete;

protected:
    virtual void OnClientRecv(BaseSocketPtr _sock);
    virtual void OnClientClose(BaseSocketPtr _sock);
    virtual void OnAccept(BaseSocketPtr _sock);

private:
    virtual void OnRecvData(BaseSocketPtr _sock) override;
    virtual void OnClose(BaseSocketPtr _sock) override;

protected:
    TcpSocketPtr m_listemSocket = nullptr;

private:
    RecvDataCallback m_clientRecvCallback;
    RecvDataCallback m_clientCloseCallback;
    RecvDataCallback m_acceptCallback;
};
} // namespace neapu

#endif // __TCPSERVER_H__