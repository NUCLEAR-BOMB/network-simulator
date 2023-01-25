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

net::Device::port_type net::Device::create_port(CIDR_type device_cidr) const noexcept
{
	return port_type(std::move(device_cidr), m_process_in_packet);
}

void net::Device::add_port(CIDR_type device_cidr, std::weak_ptr<port_type> other_port) noexcept
{
	m_connetions.push_back(
		std::make_pair(
			std::make_shared<port_type>(this->create_port(std::move(device_cidr))),
			std::move(other_port)
		)
	);
}

void net::Device::add_connection(Device& other, CIDR_type device_cidr, CIDR_type other_cidr) noexcept
{
	auto device_port = std::make_shared<port_type>(this->create_port(device_cidr));
	auto other_port  = std::make_shared<port_type>(other.create_port(other_cidr));

	m_connetions.push_back(std::make_pair(
		device_port,
		other_port
	));

	other.m_connetions.push_back(std::make_pair(
		other_port,
		device_port
	));
}

void net::Device::send(const ip_type& dest)
{
	auto res = m_arptable.find(dest);
	if (res != m_arptable.end()) 
	{
		res->second.to.send(res->second.from, net::Packet(res->second.to.ip(), dest,
			std::make_unique<net::TCP>(12345, 54321, res->second.mac)
		));
	}
	else {
		this->arp_request(dest);
		
		if (m_arptable.find(dest) == m_arptable.end()) return;

		this->send(dest);
	}
}


void net::Device::arp_request(const ip_type& dest) noexcept
{
	this->iterate_connections([&](wire_type wire) 
	{
		wire.to.send(wire.from, net::Packet(wire.to.ip(), dest,
			std::make_unique<net::ARP>(
				net::ARP::Operation::Request,
				wire.to.mac(), net::BROADCAST_MAC
			)
		));
	});
}

net::Device::ip_type net::Device::subnet(const port_type& port) const noexcept {
	return port.cidr().subnet();
}

const net::Device::port_type& net::Device::port(std::size_t index) const {
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
		if (std::shared_ptr<port_type> conn_second = conn->second.lock()) {
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
			wire.to.send(wire.from, net::Packet(wire.to.ip(), packet.source(),
				std::make_unique<net::ARP>(
					net::ARP::Operation::Reply, wire.to.mac(), arp_payload->source_mac()
				)
			));
		} else
		if (arp_payload->operation_code() == net::ARP::Operation::Reply)
		{
			m_arptable.emplace(packet.source(), arptable_mapped_t{ wire.to, wire.from, arp_payload->source_mac()});
		}
	}

	this->process_packet(wire, packet);
}
