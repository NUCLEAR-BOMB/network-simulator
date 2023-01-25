#include "Device.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>

using namespace std::placeholders;

net::Device::Device() noexcept
	: m_process_in_packet([&](net::Port& in, net::Port& from, const net::Packet& pack) {
		this->pre_process_packet(wire_type{in, from}, pack);
	})
{}

net::Device::~Device() noexcept {}

net::Port net::Device::create_port(net::CIDR device_cidr) const noexcept
{
	return { std::move(device_cidr), m_process_in_packet };
}

void net::Device::add_port(net::CIDR device_cidr, std::weak_ptr<net::Port> other_port) noexcept
{
	m_connetions.push_back(
		std::make_pair(
			std::make_shared<net::Port>(this->create_port(std::move(device_cidr))),
			std::move(other_port)
		)
	);
}

void net::Device::add_connection(Device& other, net::CIDR device_cidr, net::CIDR other_cidr) noexcept
{
	auto device_port = std::make_shared<net::Port>(this->create_port(device_cidr));
	auto other_port  = std::make_shared<net::Port>(other.create_port(other_cidr));

	m_connetions.emplace_back(device_port, other_port);
	other.m_connetions.emplace_back(other_port, device_port);
}

void net::Device::send(const net::IP& dest)
{
	auto res = m_arptable.find(dest);
	if (res != m_arptable.end()) 
	{
		this->send_payload(dest, {res->second.to, res->second.from}, std::make_unique<net::TCP>(
			12345, 54321, res->second.mac
		));
	}
	else {
		this->arp_request(dest);
		
		if (m_arptable.find(dest) == m_arptable.end()) return;

		this->send(dest);
	}
}


void net::Device::arp_request(const net::IP& dest) noexcept
{
	this->iterate_connections([&](wire_type wire) 
	{
		this->send_payload(dest, wire, std::make_unique<net::ARP>(
			net::ARP::Operation::Request,
			wire.to.mac(), net::BROADCAST_MAC
		));
	});
}

void net::Device::send_payload(const net::IP& dest, wire_type wire, std::unique_ptr<net::Packet::Payload>&& payload) noexcept
{
	wire.to.send(wire.from, net::Packet(wire.to.ip(), dest, std::move(payload)));
}

net::IP net::Device::subnet(const net::Port& port) const noexcept {
	return port.cidr().subnet();
}

const net::Port& net::Device::port(std::size_t index) const {
	return *m_connetions.at(index).first;
}


void net::Device::process_packet(wire_type wire, const net::Packet& packet)
{
	if (wire.to.ip() != packet.dest()) return;

	const auto* tcp_payload = dynamic_cast<const TCP*>(packet.payload());
	if (tcp_payload == nullptr) throw std::runtime_error("Wrong packet payload type");

	std::cout 
		<< "source: " << packet.source().to_string()
		<< "\tdest: " << packet.dest().to_string()
		<< "\tport: " << tcp_payload->dest_port() << '\n';
}

void net::Device::iterate_connections(std::function<void(wire_type)>&& func)
{
	for (auto conn = m_connetions.begin(); conn != m_connetions.end();)
	{
		if (std::shared_ptr<net::Port> conn_second = conn->second.lock()) {
			func(wire_type{*(conn->first), *(conn_second)});
		}
		else {
			conn = m_connetions.erase(conn); continue;
		}
		++conn;
	}
}

void net::Device::pre_process_packet(wire_type wire, const net::Packet& packet)
{
	const auto* arp_payload = dynamic_cast<const ARP*>(packet.payload());

	if (arp_payload != nullptr && (wire.to.ip() == packet.dest()))
	{
		if (arp_payload->operation_code() == net::ARP::Operation::Request) 
		{
			this->send_payload(packet.source(), wire, std::make_unique<net::ARP>(
				net::ARP::Operation::Reply, wire.to.mac(), arp_payload->source_mac()
			));
		} else
		if (arp_payload->operation_code() == net::ARP::Operation::Reply)
		{
			m_arptable.emplace(packet.source(), arptable_mapped_t{ wire.to, wire.from, arp_payload->source_mac()});
		}
	}

	this->process_packet(wire, packet);
}
