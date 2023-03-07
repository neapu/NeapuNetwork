#ifndef __BASESOCKET_H__
#define __BASESOCKET_H__

#include "NetworkDef.h"
#include "ModuleLoggerDef.h"

namespace neapu{
class BaseSocket{
public:
    void SetLogCallback(LogCallback _cb);

protected:
    static int GetErrorCode();

protected:

private:
    LogCallback m_logCallback;
};
}


#endif // __BASESOCKET_H__