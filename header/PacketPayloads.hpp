#pragma once

#include "Packet.hpp"

namespace net
{

class TCP : public net::Packet::Payload
{
public:
	using tcp_port_type = unsigned int;
	using MAC_type = net::MAC;

	TCP(tcp_port_type tcp_port, const MAC_type& mac) noexcept;

	tcp_port_type port() const noexcept;
	const MAC_type& mac() const noexcept;

private:
	tcp_port_type m_port;
	const MAC_type& m_mac;
};

class ICMP : public net::Packet::Payload
{
public:

private:

};

}