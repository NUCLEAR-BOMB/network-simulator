#pragma once

#include "PacketPayloads.hpp"

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

	using connections_type = std::vector<std::pair<std::shared_ptr<port_type>, std::weak_ptr<port_type>>>;

	Device() noexcept;

	virtual ~Device() noexcept;

	port_type create_port(CIDR_type device_cidr) const noexcept;
	
	void add_port(CIDR_type device_cidr, std::weak_ptr<port_type> other_port) noexcept;

	void add_connection(Device& other, CIDR_type device_cidr, CIDR_type other_cidr) noexcept;

	void send(const ip_type& dest);

	ip_type subnet(const port_type& port) const noexcept;

	const port_type& port(std::size_t index) const;

private:

	virtual void process_packet(net::Port& in_port, const net::Packet& packet);

	connections_type m_connetions;
	typename port_type::recive_function_type m_process_in_packet;
};

}