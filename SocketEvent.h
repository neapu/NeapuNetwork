#ifndef __SOCKETEVENT_H__
#define __SOCKETEVENT_H__

#include "BaseSocket.h"
#include <functional>
#include <map>
#include <mutex>

namespace neapu {
using RecvDataCallback = std::function<void(BaseSocketPtr _sock)>;

class NEAPU_NETWORK_EXPORT SocketEvent {
public:
    void OnRecvData(RecvDataCallback _cb);
    void OnClose(RecvDataCallback _cb);
    int SetSocket(BaseSocketPtr _sock);
    int Loop();

protected:
    virtual void OnRecvData(BaseSocketPtr _sock);
    virtual void OnClose(BaseSocketPtr _sock);

protected:
    std::map<SOCKET_FD, BaseSocketPtr> m_socketList;
    std::mutex m_socketListMutex;

    RecvDataCallback m_recvCallback;
    RecvDataCallback m_closeCallback;

    bool m_stopLoop = true;
};

} // namespace neapu

#endif // __SOCKETEVENT_H__