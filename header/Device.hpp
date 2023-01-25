#pragma once

#include "PacketPayloads.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <map>

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

protected:

	struct wire_type {
		net::Port& to;
		net::Port& from;
	};

	void arp_request(const ip_type& dest) noexcept;

	virtual void process_packet(wire_type wire, const net::Packet& packet);

	void iterate_connections(std::function<void(wire_type)>&& func);

	struct arptable_mapped_t {
		net::Port& to;
		net::Port& from;
		const net::MAC& mac;
	};

	std::map<net::IP, arptable_mapped_t> m_arptable;
private:

	void pre_process_packet(wire_type wire, const net::Packet& packet);

	connections_type m_connetions;
	typename port_type::recive_function_type m_process_in_packet;
};

}