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
	using CIDR_type = net::CIDR;

	using ip_type = typename CIDR::ip_type;
	using ip_mask_type = typename CIDR::ip_mask_type;

	using port_type = net::Port;

	using connections_type = std::vector<std::reference_wrapper<const port_type>>;

	Device(CIDR_type cidr) noexcept;

	void add_port(const port_type& other_port) noexcept;
	void add_connection(Device& other) noexcept;

	const ip_type& ip() const noexcept;
	const ip_mask_type& mask() const noexcept;

	const port_type& port() const noexcept;

	void send(const ip_type& dest) const noexcept;

	ip_type subnet() const noexcept;

private:

	bool filter_ip(const net::IP& ip) const noexcept;

	void use_in_packet(const net::Packet& packet) noexcept;

	net::CIDR m_cidr;

	connections_type m_connetions;
	port_type m_device_port;
};

}