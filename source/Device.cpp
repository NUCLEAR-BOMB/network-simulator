#include "Device.hpp"
#include "Logging.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>

using namespace std::placeholders;

net::Device::Device() noexcept
	: m_process_in_packet([&](net::Interface& in, net::Interface& from, net::Packet pack) {
		this->preprocess_packet(wire_type{in, from}, std::move(pack));
	})
{}

net::Device::~Device() noexcept {}

net::Interface net::Device::create_port(net::CIDR device_cidr) const noexcept
{
	return { std::move(device_cidr), m_process_in_packet };
}

void net::Device::add_port(net::CIDR device_cidr, std::weak_ptr<net::Interface> other_port) noexcept
{
	m_connetions.push_back(
		std::make_pair(
			std::make_shared<net::Interface>(this->create_port(std::move(device_cidr))),
			std::move(other_port)
		)
	);
}

void net::Device::add_connection(Device& other, net::CIDR device_cidr, net::CIDR other_cidr) noexcept
{
	auto device_port = std::make_shared<net::Interface>(this->create_port(device_cidr));
	auto other_port  = std::make_shared<net::Interface>(other.create_port(other_cidr));

	m_connetions.emplace_back(device_port, other_port);
	other.m_connetions.emplace_back(other_port, device_port);
}

void net::Device::send(const net::IP& dest)
{
	this->send_payload(dest, std::make_unique<net::TCP>(
		12345, 54321
	));
}


void net::Device::arp_request(const net::IP& dest) noexcept
{
	LOG("Sending ARP request to %s", dest.to_string().c_str());
	this->iterate_connections([&](wire_type wire) 
	{
		this->send_payload_to_wire(net::BROADCAST_MAC, dest, wire, std::make_unique<net::ARP>(
			net::ARP::Operation::Request,
			wire.to.mac(), net::BROADCAST_MAC
		));
	});
}

void net::Device::send_payload_to_wire(const net::MAC& dest, const net::IP& ip_dest, wire_type wire, std::unique_ptr<net::Packet::Payload>&& payload) noexcept
{
	wire.to.send(wire.from,
		net::Packet(wire.to.mac(), dest, wire.to.ip(), ip_dest, std::move(payload))
	);
}

void net::Device::send_payload(const net::IP& ip_dest, std::unique_ptr<net::Packet::Payload>&& payload)
{
	LOG("Trying send a packet to %s", ip_dest.to_string().c_str());

	auto arptable_res = m_arptable.find(ip_dest);

	if (arptable_res != m_arptable.end())
	{
		LOG("%s | Sending packet...", arptable_res->second.wire.to.ip().to_string().c_str());
		this->send_payload_to_wire(
			arptable_res->second.mac, ip_dest, arptable_res->second.wire, std::move(payload)
		);
	}
	else {
		LOG("No mac address exists in the ARP table. Requesting the MAC address...");
		this->arp_request(ip_dest);

		if (m_arptable.find(ip_dest) == m_arptable.end()) {
			LOG("Failed to request MAC address");
			return;
		}

		LOG("Mac address requested. Trying to send package again");
		this->send_payload(ip_dest, std::move(payload));
	}
}

net::IP net::Device::subnet(const net::Interface& port) const noexcept {
	return port.cidr().subnet();
}

const net::Interface& net::Device::port(std::size_t index) const {
	return *m_connetions.at(index).first;
}


void net::Device::process_packet(wire_type wire, net::Packet packet)
{
	if (wire.to.ip() != packet.ip_dest()) return;

	const auto* tcp_payload = dynamic_cast<const TCP*>(packet.payload());
	if (tcp_payload == nullptr) throw std::runtime_error("Wrong packet payload type");

	std::cout 
		<< "source: " << packet.ip_source().to_string()
		<< "\tdest: " << packet.ip_dest().to_string()
		<< "\tport: " << tcp_payload->dest_port() << '\n';
}

void net::Device::iterate_connections(std::function<void(wire_type)>&& func)
{
	for (auto conn = m_connetions.begin(); conn != m_connetions.end();)
	{
		if (std::shared_ptr<net::Interface> conn_second = conn->second.lock()) {
			func({*(conn->first), *(conn_second)});
		}
		else {
			conn = m_connetions.erase(conn); continue;
		}
		++conn;
	}
}

bool net::Device::verify_in_packet(const wire_type wire, const net::Packet& packet) const noexcept
{
	if (wire.to.ip() != packet.ip_dest()) return false;
	if (wire.to.mac() != packet.mac_dest()) return false;
	if (this->subnet(wire.to) != (wire.to.mask() & packet.ip_dest())) return false;

	return true;
}

void net::Device::preprocess_packet(wire_type wire, net::Packet packet)
{
	LOG("%s | Preprocessing packet from %s", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
	const auto* arp_payload = dynamic_cast<const ARP*>(packet.payload());

	if (arp_payload != nullptr && (wire.to.ip() == packet.ip_dest()))
	{
		if (arp_payload->operation_code() == net::ARP::Operation::Request) 
		{
			LOG("%s | Sending an ARP reply packet to %s", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());

			this->send_payload_to_wire(packet.mac_source(), packet.ip_source(), wire, std::make_unique<net::ARP>(
				net::ARP::Operation::Reply, wire.to.mac(), arp_payload->source_mac()
			));
		} else
		if (arp_payload->operation_code() == net::ARP::Operation::Reply)
		{
			LOG("%s | Adding MAC address to ARP table: IP %s", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
			m_arptable.emplace(packet.ip_source(), arptable_mapped_t{ wire, arp_payload->source_mac()});
		}
	}

	this->process_packet(wire, std::move(packet));
}
