#ifndef __NETWORKDEF_H__
#define __NETWORKDEF_H__

#include <basetsd.h>
#include <functional>

#ifdef _WIN32
#ifdef NEAPU_NETWORK_EXPORTS
#define NEAPU_NETWORK_EXPORT __declspec(dllexport)
#else
#define NEAPU_NETWORK_EXPORT __declspec(dllimport)
#endif
#else
#endif

namespace neapu {
enum class IP_TYPE {
    NONE = 0,
    IPv4 = 0x01,
    IPv6 = 0x02,
};

constexpr int INVALID_SOCKET = -1;

#ifdef _WIN32
using SOCKET_FD = UINT_PTR;
#else
using SOCKET_FD = int;
#endif

}

#endif // __NETWORKDEF_H__