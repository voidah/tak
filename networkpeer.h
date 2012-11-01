#ifndef TAK_NETWORKPEER_H__
#define TAK_NETWORKPEER_H__

#include <string>

class Network;

class NetworkPeer
{
    public:
        NetworkPeer();
        NetworkPeer(void* peer);

        int GetPing() const;
        bool IsConnected() const;

        std::string GetIp() const;
        unsigned short GetPort() const;

    private:
        void* m_peer;

        friend class Network;
};


#endif // TAK_NETWORKPEER_H__
