#pragma once

#include "Packet.hpp"

#include <vector>
#include <utility>
#include <functional>

namespace net
{

class Device
{
public:
	using ip_type = net::IP;
	using mac_type = net::MAC;
	using port_type = net::Port;

	using connections_type = std::vector<std::reference_wrapper<const port_type>>;

	Device(ip_type ip) noexcept;

	void add_port(const port_type& other_port) noexcept;

	const ip_type& ip() const noexcept;
	const port_type& port() const noexcept;

	void send(const ip_type& dest) const noexcept;

private:

	static void use_in_packet(const net::Packet& packet) noexcept;

	ip_type m_ip;
	mac_type m_mac;

	connections_type m_connetions;
	port_type m_device_port;
};

}