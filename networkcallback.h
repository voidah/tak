#ifndef NETWORKCALLBACK_H
#define NETWORKCALLBACK_H

#include "define.h"

#ifdef TAK_USE_ENET

#include "network.h"

class NetworkCallback
{
    public:
        virtual void OnNetworkConnect(const NetworkPeer& peer, void*& userData) = 0;
        virtual void OnNetworkDisconnect(const NetworkPeer& peer, void* userData) = 0;
        virtual void OnNetworkReceive(const NetworkPeer& peer, const char* data, int len, void* userData) = 0;
};

#endif // TAK_USE_ENET

#endif // NETWORKCALLBACK_H
