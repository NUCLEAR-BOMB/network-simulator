#pragma once

#include "Packet.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <memory>

namespace net
{

class Device
{
public:
	using CIDR_type = net::CIDR;

	using ip_type = typename CIDR::ip_type;
	using ip_mask_type = typename CIDR::ip_mask_type;

	using port_type = net::Port;

	using connections_type = std::vector<std::pair<std::unique_ptr<port_type>, port_type*>>;

	Device() noexcept;

	virtual ~Device() noexcept;

	port_type create_port(CIDR_type device_cidr) const noexcept;
	
	void add_port(CIDR_type device_cidr, port_type* other_port) noexcept;

	void add_connection(CIDR_type device_cidr, CIDR_type other_cidr, Device& other) noexcept;

	void send(const ip_type& dest) noexcept;

	ip_type subnet(const port_type& port) const noexcept;

	const port_type& port(std::size_t index) const;

private:

	bool filter_ip(net::Port& in_port, const net::IP& ip) const noexcept;

	void use_in_packet(net::Port& in_port, const net::Packet& packet) noexcept;

	connections_type m_connetions;
	typename port_type::recive_function_type m_process_in_packet;
};

}