#include "define.h"

#ifdef TAK_USE_ENET
#include "network.h"
#include "autolock.h"
#include <iostream>
#include <cstring>
#include <cstdarg>
#include "networkcallback.h"
#include <enet/enet.h>

#define NETWORK_CHANNEL_COUNT 2

Network::Network() : m_type(NONE), m_host(0), m_networkCallback(0)
{
}

Network::~Network()
{
}

bool Network::Init(NETWORK_TYPE networkType)
{
    AutoLock<Mutex> lock(m_mutex);

    if(networkType == NONE)
        return false;

    if(enet_initialize() != 0)
    {
        std::cout << "Error initializing network" << std::endl;
        return false;
    }

    if(networkType == CLIENT)
    {
        m_host = enet_host_create (NULL, 1, NETWORK_CHANNEL_COUNT, 0, 0);

        if(!m_host)
        {
            std::cout << "Error initializing connection" << std::endl;
            return false;
        }
    }

    m_type = networkType;
    return true;
}

void Network::DeInit()
{
    AutoLock<Mutex> lock(m_mutex);

    if(m_host)
        enet_host_destroy((ENetHost*)m_host);
    enet_deinitialize();
}

bool Network::Tick()
{
    AutoLock<Mutex> lock(m_mutex);

    if(!m_host)
        return false;

    if(!m_networkCallback)
        return false;

    ENetEvent event;

    while(enet_host_service((ENetHost*)m_host, &event, 0) > 0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                m_networkCallback->OnNetworkConnect(NetworkPeer(event.peer), event.peer->data);
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                m_networkCallback->OnNetworkReceive(NetworkPeer(event.peer), (const char*)event.packet->data, event.packet->dataLength, event.peer->data);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                if(m_type == CLIENT)
                    m_host = 0;
                m_networkCallback->OnNetworkDisconnect(NetworkPeer(event.peer), event.peer->data);
                event.peer->data = NULL;

            default:
                break;
        }

        return true; // event processed
    }

    return false;
}

bool Network::Listen(unsigned short port, NetworkCallback* callback)
{
    if(m_type != SERVER)
        return false;

    m_networkCallback = callback;

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    m_host = enet_host_create(&address, 256, NETWORK_CHANNEL_COUNT, 0, 0);

    if(!m_host)
    {
        std::cout << "Error listening on connection" << std::endl;
        return false;
    }

    return true;
}

NetworkPeer Network::Connect(const std::string& host, unsigned short port, NetworkCallback* callback)
{
    AutoLock<Mutex> lock(m_mutex);

    if(!m_host)
        return NetworkPeer(0);

    if(m_type != CLIENT)
    {
        std::cout << "Connect operation valid only if type is client" << std::endl;
        return NetworkPeer(0);
    }

    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    ENetPeer* peer = enet_host_connect((ENetHost*)m_host, &address, NETWORK_CHANNEL_COUNT, 0);    

    if(!peer)
    {
        std::cout << "Failed to connect to " << host << ":" << port << std::endl;
        return NetworkPeer(0);
    }

    m_networkCallback = callback;

    ENetEvent event;
    if(enet_host_service((ENetHost*)m_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
        return NetworkPeer(peer);

    enet_peer_reset(peer);
    std::cout << "Failed to connect to " << host << ":" << port << std::endl;
    return NetworkPeer(0);
}

void Network::Disconnect(const NetworkPeer& peer)
{
    AutoLock<Mutex> lock(m_mutex);

    if(!peer.IsConnected())
        return;

    enet_peer_disconnect((ENetPeer*)peer.m_peer, 0);
    enet_host_flush((ENetHost*)m_host);

    ENetEvent event;
    enet_host_service((ENetHost*)m_host, &event, 5000);
}

bool Network::SendTo(const NetworkPeer& peer, bool reliable, const void* data, int len)
{
    AutoLock<Mutex> lock(m_mutex);

    if(!m_host)
        return false;

    if(!peer.IsConnected())
        return false;

    if(len == 0)
        len = strlen((const char*)data);

    ENetPacket* packet = enet_packet_create(data, len, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

    // Channel 1 if the data should be 'reliable' (if the packet should be ack'ed by
    // the remote host), or channel 0 if we don't care if the data is lost
    int ret = enet_peer_send((ENetPeer*)peer.m_peer, reliable ? 1 : 0, packet);

    return ret == 0;
}

bool Network::SendTo(const NetworkPeer& peer, bool reliable, const std::string& data)
{
    return SendTo(peer, reliable, data.c_str(), data.length());
}

bool Network::SendfTo(const NetworkPeer& peer, bool reliable, const char* fmt, int len, ...)
{
    if(!peer.IsConnected())
        return false;

    char *buffer = new char[len];

    va_list args;
    va_start(args, len);
    int rlen = vsnprintf(buffer, len, fmt, args);
    va_end(args);

    bool ret = SendTo(peer.m_peer, reliable, buffer, rlen);
    delete [] buffer;
    return ret;
}

bool Network::Broadcast(bool reliable, const void* data, int len)
{
    AutoLock<Mutex> lock(m_mutex);

    if(!m_host)
        return false;

    if(m_type != SERVER)
    {
       std::cout << "Broadcast operation valid only if type is server" << std::endl;
       return false;
    }

    if(len == -1)
        len = strlen((const char*)data);

    ENetPacket* packet = enet_packet_create(data, len, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);

    enet_host_broadcast((ENetHost*)m_host, reliable ? 1 : 0, packet);

    return true;
}

bool Network::Broadcastf(bool reliable, const char* fmt, int len, ...)
{
    char *buffer = new char[len];

    va_list args;
    va_start(args, len);
    int rlen = vsnprintf(buffer, len, fmt, args);
    va_end(args);

    bool ret = Broadcast(reliable, buffer, rlen);
    delete [] buffer;
    return ret;
}

#endif // TAK_USE_ENET
