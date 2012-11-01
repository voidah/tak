#ifndef NETWORK_H
#define NETWORK_H

#include "define.h"

#ifdef TAK_USE_ENET

#include <cstdio>
#include <string>
#include "mutex.h"
#include "networkpeer.h"

#define GNetwork (*Network::Instance())

class NetworkCallback;

class Network // Singleton
{
    public:
        static Network* Instance()
        {
            static Network inst;
            return &inst;
        }

        enum NETWORK_TYPE {NONE, SERVER, CLIENT};

        bool Init(NETWORK_TYPE networkType);
        void DeInit();

        // The Tick method should be called for the various callback to be
        // called (to be able to receive, detect connection, etc)
        bool Tick();

        bool Listen(unsigned short port, NetworkCallback* callback);

        NetworkPeer Connect(const std::string& host, unsigned short port, NetworkCallback* callback);
        void Disconnect(const NetworkPeer& peer);

        bool SendTo(const NetworkPeer& peer, bool reliable, const void* data, int len);
        bool SendTo(const NetworkPeer& peer, bool reliable, const std::string& data);
        bool SendfTo(const NetworkPeer& peer, bool reliable, const char* fmt, int len, ...);
        bool Broadcast(bool reliable, const void* data, int len = -1);
        bool Broadcastf(bool reliable, const char* fmt, int len, ...);

    protected:
        Network();
        ~Network();
        Network(const Network&);
        Network& operator=(const Network&);

    private:
        NETWORK_TYPE m_type;
        void* m_host;
        NetworkCallback* m_networkCallback;

        Mutex m_mutex;
};

#endif // TAK_USE_ENET

#endif // NETWORK_H
