#ifdef TAK_USE_ENET

#include "networkpeer.h"
#include <enet/enet.h>
#include <sstream>

NetworkPeer::NetworkPeer() : m_peer(0)
{
}

NetworkPeer::NetworkPeer(void* peer) : m_peer(peer)
{
}

int NetworkPeer::GetPing() const
{
    if(!m_peer)
        return -1;

    return ((ENetPeer*)m_peer)->roundTripTime;
}

bool NetworkPeer::IsConnected() const
{
    if(!m_peer)
        return false;

    return (((ENetPeer*)m_peer)->state == ENET_PEER_STATE_CONNECTED);
}

std::string NetworkPeer::GetIp() const
{
    enet_uint32 ip = ((ENetPeer*)m_peer)->address.host;

    std::ostringstream ipStr;
    ipStr << (ip & 0xff);
    ipStr << "." << ((ip & 0xff00) >> 8);
    ipStr << "." << ((ip & 0xff0000) >> 16);
    ipStr << "." << ((ip & 0xff000000) >> 24);

    return ipStr.str();
}

unsigned short NetworkPeer::GetPort() const
{
    return ((ENetPeer*)m_peer)->address.port;
}

#endif // TAK_USE_ENET
