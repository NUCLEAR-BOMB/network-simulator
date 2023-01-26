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
	using connections_type = std::vector<std::pair<std::shared_ptr<net::Port>, std::weak_ptr<net::Port>>>;

	Device() noexcept;

	virtual ~Device() noexcept;

	net::Port create_port(net::CIDR device_cidr) const noexcept;
	
	void add_port(net::CIDR device_cidr, std::weak_ptr<net::Port> other_port) noexcept;

	void add_connection(Device& other, net::CIDR device_cidr, net::CIDR other_cidr) noexcept;

	void send(const net::IP& dest);

	net::IP subnet(const net::Port& port) const noexcept;

	const net::Port& port(std::size_t index) const;

protected:

	struct wire_type {
		net::Port& to;
		net::Port& from;
	};

	void arp_request(const net::IP& dest) noexcept;

	void send_payload_to_wire(const net::MAC& dest, const net::IP& ip_dest, wire_type wire, std::unique_ptr<net::Packet::Payload>&& payload) noexcept;
	void send_payload(const net::IP& ip_dest, std::unique_ptr<net::Packet::Payload>&& payload);

	virtual void process_packet(wire_type wire, net::Packet packet);

	void iterate_connections(std::function<void(wire_type)>&& func);

	bool verify_in_packet(const wire_type wire, const net::Packet& packet) const noexcept;

	struct arptable_mapped_t {
		//net::Port& to;
		//net::Port& from;
		wire_type wire;
		const net::MAC mac;
	};

	std::map<net::IP, arptable_mapped_t> m_arptable;
private:

	void preprocess_packet(wire_type wire, net::Packet packet);

	connections_type m_connetions;
	typename net::Port::recive_function_type m_process_in_packet;
};

}