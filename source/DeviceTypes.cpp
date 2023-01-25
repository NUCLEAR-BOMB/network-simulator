#include "DeviceTypes.hpp"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

void net::Computer::process_packet(wire_type wire, net::Packet packet)
{
	if (wire.to.ip() != packet.dest()) return;

	const auto* tcp_payload = dynamic_cast<const net::TCP*>(packet.payload());
	if (tcp_payload == nullptr) return;

	std::cout 
		<< wire.to.ip().to_string() << " - "
		<< "source ip: " << packet.source().to_string() 
		<< "\tdest ip: " << packet.dest().to_string()
		<< "\ttcp port: " << tcp_payload->dest_port()
		<< "\ttcp mac: " << tcp_payload->mac().to_string() << '\n';
}


void net::Switch::resend(wire_type wire, net::Packet packet) const
{
	wire.to.send(wire.from, std::move(packet));
}

void net::Switch::process_packet(wire_type wire, net::Packet packet)
{
	const auto* arp_payload = dynamic_cast<const net::ARP*>(packet.payload());

	if (arp_payload != nullptr) 
	{
		m_arptable.emplace(packet.source(), arptable_mapped_t{ wire.to, wire.from, arp_payload->source_mac() });

		if (arp_payload->operation_code() == net::ARP::Operation::Request) 
		{
			this->iterate_connections([=](wire_type connection_wire) {
				this->resend(connection_wire, std::move(packet));
			});
			return;
		}
	}

	auto find_res = m_arptable.find(packet.dest());
	if (find_res != m_arptable.end()) 
	{
		this->resend({find_res->second.to, find_res->second.from}, std::move(packet));
	}
	else {
		throw std::runtime_error("Unknown device");
	}
}
