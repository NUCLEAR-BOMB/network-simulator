#include "Device.hpp"
#include "Logging.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>

using namespace std::placeholders;

net::Device::Device() noexcept
	: m_process_in_packet([&](net::Interface& interface, net::Packet pack) {
		this->preprocess_packet(interface, std::move(pack));
	})
{}

net::Device::~Device() noexcept {}

net::Interface net::Device::create_port(net::CIDR device_cidr) const noexcept
{
	return { std::move(device_cidr), m_process_in_packet };
}

void net::Device::add_connection(Device& other, net::CIDR device_cidr, net::CIDR other_cidr) noexcept
{
	//auto device_port = std::make_shared<net::Interface>(this->create_port(device_cidr));
	//auto other_port  = std::make_shared<net::Interface>(other.create_port(other_cidr));

	//m_connetions.emplace_back(device_port, other_port);
	//other.m_connetions.emplace_back(other_port, device_port);

	auto device_interface = this->create_port(std::move(device_cidr));
	auto another_port = other.create_port(std::move(other_cidr));

	device_interface.connect_to(&another_port);
	another_port.connect_to(&device_interface);

	m_connetions.push_back(std::move(device_interface));
	other.m_connetions.push_back(std::move(another_port));
}

void net::Device::send(const net::IP& dest)
{
	this->send_payload(dest, std::make_unique<net::TCP>(
		12345, 54321
	));
}

void net::Device::routingtable_add_back(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept {
	m_routingtable.add_back(ip, mask, interface);
}

void net::Device::routingtable_add_front(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept {
	m_routingtable.add_front(ip, mask, interface);
}


void net::Device::arp_request(const net::IP& dest) noexcept
{
	LOG("Sending ARP request to %s", dest.to_string().c_str());

	for (auto& interface : m_connetions)
	{
		this->send_payload_to_interface(net::BROADCAST_MAC, dest, interface, 
			std::make_unique<net::ARP>(
				net::ARP::Operation::Request,
				interface.mac(), net::BROADCAST_MAC
			)
		);
	}
}

void net::Device::send_payload_to_interface(const net::MAC& dest, const net::IP& ip_dest, net::Interface& interface, std::unique_ptr<net::Packet::Payload>&& payload) noexcept
{
	bool send_res = interface.send(
		net::Packet(interface.mac(), dest, interface.ip(), ip_dest, std::move(payload))
	);

	if (!send_res) {
		LOG("Can't send a packet payload to %s", ip_dest.to_string().c_str());
	}
}

void net::Device::send_payload(const net::IP& ip_dest, std::unique_ptr<net::Packet::Payload>&& payload)
{
	LOG("Trying send a packet to %s", ip_dest.to_string().c_str());

	auto* mac_dest = m_arptable.find(ip_dest);

	if (mac_dest)
	{
		//auto& interface = this->find_interface(ip_dest);

		auto& rountintable_res = m_routingtable.find(ip_dest);
		auto& interface = rountintable_res.interface;

		LOG("%s | Sending packet...", interface.ip().to_string().c_str());

		this->send_payload_to_interface(
			*mac_dest, ip_dest, interface, std::move(payload)
		);
	}
	else {
		LOG("No mac address exists in the ARP table. Requesting the MAC address...");
		this->arp_request(ip_dest);

		if (!m_arptable.find(ip_dest)) {
			LOG("Failed to request MAC address");
			return;
		}

		LOG("Mac address requested. Trying to send package again");
		this->send_payload(ip_dest, std::move(payload));
	}
}

void net::Device::send_packet(const net::IP& ip_dest, net::Packet packet) noexcept
{
	auto& res = this->m_routingtable.find(ip_dest);
	auto& interface = res.interface;

	LOG("%s | Sending packet to %s ...", interface.ip().to_string().c_str(), ip_dest.to_string().c_str());

	auto send_res = interface.send(std::move(packet));

	if (!send_res) {
		LOG("Can't send a packet to %s", ip_dest.to_string().c_str());
	}
}

net::IP net::Device::subnet(const net::Interface& inter) const noexcept {
	return inter.cidr().subnet();
}

const net::Interface& net::Device::port(std::size_t index) const {
	return m_connetions.at(index);
}


void net::Device::process_packet(net::Interface& interface, net::Packet packet)
{
	if (interface.ip() != packet.ip_dest()) return;

	const auto* tcp_payload = dynamic_cast<const TCP*>(packet.payload());
	if (tcp_payload == nullptr) throw std::runtime_error("Wrong packet payload type");

	std::cout 
		<< "source: " << packet.ip_source().to_string()
		<< "\tdest: " << packet.ip_dest().to_string()
		<< "\tport: " << tcp_payload->dest_port() << '\n';
}

bool net::Device::verify_in_packet(const net::Interface& interface, const net::Packet& packet) const noexcept
{
	if (interface.ip() != packet.ip_dest()) return false;
	if (interface.mac() != packet.mac_dest()) return false;
	if (this->subnet(interface) != (interface.mask() & packet.ip_dest())) return false;

	return true;
}

void net::Device::preprocess_packet(net::Interface& interface, net::Packet packet)
{
	LOG("%s | Preprocessing packet from %s", interface.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
	const auto* arp_payload = dynamic_cast<const ARP*>(packet.payload());

	if (arp_payload != nullptr && (interface.ip() == packet.ip_dest()))
	{
		LOG("%s | Adding MAC address to ARP table: IP %s", interface.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
		m_arptable.add_back(packet.ip_source(), arp_payload->source_mac());

		LOG("%s | Adding interface to routing table", interface.ip().to_string().c_str());
		m_routingtable.add_back(packet.ip_source(), net::IPMask{255,255,255,255}, interface);

		if (arp_payload->operation_code() == net::ARP::Operation::Request) 
		{
			LOG("%s | Sending an ARP reply packet to %s", interface.ip().to_string().c_str(), packet.ip_source().to_string().c_str());

			this->send_payload_to_interface(packet.mac_source(), packet.ip_source(), interface, std::make_unique<net::ARP>(
				net::ARP::Operation::Reply, interface.mac(), arp_payload->source_mac()
			));
		}
	}

	this->process_packet(interface, std::move(packet));
}
