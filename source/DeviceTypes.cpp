#include "DeviceTypes.hpp"

#include "Logging.hpp"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

void net::Computer::process_packet(wire_type wire, net::Packet packet)
{
	if (wire.to.ip() != packet.dest()) return;

	const auto* tcp_payload = dynamic_cast<const net::TCP*>(packet.payload());
	if (tcp_payload == nullptr) return;

	LOG("%s | Get packet!", wire.to.ip().to_string().c_str());

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
		#ifdef ENABLE_LOGGING
		auto arptable_res =
		#endif
		m_arptable.emplace(packet.source(), arptable_mapped_t{ wire.to, wire.from, arp_payload->source_mac() });

		#ifdef ENABLE_LOGGING
		if (arptable_res.second) {
			LOG("%s | Adding MAC address to ARP table from %s ...", wire.to.ip().to_string().c_str(), packet.source().to_string().c_str());
		}
		#endif

		if (arp_payload->operation_code() == net::ARP::Operation::Request) 
		{
			LOG("%s | Resending ARP packet to all of ports...", wire.to.ip().to_string().c_str());
			this->iterate_connections([=](wire_type connection_wire) {
				this->resend(connection_wire, std::move(packet));
			});
			return;
		}
	}

	auto find_res = m_arptable.find(packet.dest());
	if (find_res != m_arptable.end()) 
	{
		LOG("%s | Resending packet to %s ...", wire.to.ip().to_string().c_str(), find_res->second.to.ip().to_string().c_str());
		this->resend({find_res->second.to, find_res->second.from}, std::move(packet));
	}
	else {
		throw std::runtime_error("Unknown device");
	}
}
