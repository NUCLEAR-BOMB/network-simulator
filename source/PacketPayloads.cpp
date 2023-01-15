#include "PacketPayloads.hpp"

#include <cstring>

net::TCP::TCP(tcp_port_type tcp_port, const MAC_type& mac) noexcept
	: m_port(tcp_port)
	, m_mac(mac)
{}

net::TCP::tcp_port_type net::TCP::port() const noexcept {
	return m_port;
}

const net::TCP::MAC_type& net::TCP::mac() const noexcept {
	return m_mac;
}
