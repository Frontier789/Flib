#include <FRONTIER/Network/TcpListener.hpp>

namespace fn
{
    bool TcpListener::listen(const IpAddress &ip)
    {
        if (!m_socket.create(ip.isIpv6())) return false;
        if (!m_socket.bind(ip)) return false;
        if (!m_socket.listen()) return false;

        return true;
    }
    bool TcpListener::listen(const IpAddress &ip,fm::Uint16 port)
    {
        if (!m_socket.create(ip.isIpv6())) return false;
        if (!m_socket.bind(IpAddress(ip,port))) return false;
        if (!m_socket.listen()) return false;

        return true;
    }
    bool TcpListener::listen(fm::Uint16 port,bool useIpv6)
    {
        if (!m_socket.create(useIpv6)) return false;
        if (!m_socket.bind(IpAddress::localHost(port,useIpv6))) return false;
        if (!m_socket.listen()) return false;

        return true;
    }

    bool TcpListener::accept(TcpSocket &soc)
    {
        return m_socket.accept(soc);
    }

    bool TcpListener::isValid() const
    {
        return m_socket.isValid();
    }

    TcpListener::operator bool() const
    {
        return isValid();
    }
}
