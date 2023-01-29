#pragma once

#include "PacketPayloads.hpp"
#include "Tables.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <map>
#include <initializer_list>

namespace net
{

class Device
{
public:

	using connections_type = std::vector<net::Interface>;

	Device() noexcept;

	Device(net::CIDR cidr) noexcept;

	virtual ~Device() noexcept;

	net::Interface create_port(net::CIDR device_cidr) const noexcept;

	void add_connection(Device& other, net::CIDR device_cidr, net::CIDR other_cidr) noexcept;

	void connect_to(Device& other);
	void connect_to(Device& other, net::CIDR other_cidr);

	void send(const net::IP& dest);

	void routingtable_add_back(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept;
	void routingtable_add_front(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept;

	net::IP subnet(const net::Interface& port) const noexcept;

	const net::Interface& port(std::size_t index) const;

protected:

	void arp_request(const net::IP& dest) noexcept;

	void send_payload_to_interface(const net::MAC& dest, const net::IP& ip_dest, net::Interface& interface, std::unique_ptr<net::Packet::Payload>&& payload) noexcept;
	void send_payload(const net::IP& ip_dest, std::unique_ptr<net::Packet::Payload>&& payload);

	void send_packet(const net::IP& ip_dest, net::Packet packet) noexcept;

	virtual void process_packet(net::Interface& interface, net::Packet packet);

	bool verify_in_packet(const net::Interface& interface, const net::Packet& packet) const noexcept;

	//std::map<net::IP, arptable_mapped_t> m_arptable;

	net::ARPTable m_arptable;
	net::RoutingTable m_routingtable;

	connections_type m_connetions;

private:

	void preprocess_packet(net::Interface& interface, net::Packet packet);

	typename net::Interface::recive_function_type m_process_in_packet;
};

}