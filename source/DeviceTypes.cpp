#include "DeviceTypes.hpp"

#include "Logging.hpp"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

void net::Computer::process_packet(wire_type wire, net::Packet packet)
{
	if (wire.to.ip() != packet.ip_dest()) {
		LOG("%s | Ignoring wrong recipient IP address: %s", wire.to.ip().to_string().c_str(), packet.ip_dest().to_string().c_str()); 
		return;
	}

	const auto* tcp_payload = dynamic_cast<const net::TCP*>(packet.payload());
	if (tcp_payload == nullptr) {
		LOG("%s | Ignoring non-TCP packet", wire.to.ip().to_string().c_str()); 
		return;
	}

	LOG("%s | Get packet!", wire.to.ip().to_string().c_str());

	std::cout
		<< wire.to.ip().to_string() << " - "
		<< "source ip: " << packet.ip_source().to_string()
		<< "\tdest ip: " << packet.ip_dest().to_string()
		<< "\ttcp port: " << tcp_payload->dest_port() << '\n';
}


void net::Switch::resend(wire_type wire, net::Packet packet) const
{
	wire.to.send(wire.from, std::move(packet));
}

void net::Switch::process_packet(wire_type wire, net::Packet packet)
{
	LOG("%s Switch | Forwarding packet from %s", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());

	const auto* arp_payload = dynamic_cast<const net::ARP*>(packet.payload());

	if (arp_payload != nullptr) 
	{
		#ifdef ENABLE_LOGGING
		auto arptable_res =
		#endif
		m_arptable.emplace(packet.ip_source(), arptable_mapped_t{ wire.to, wire.from, arp_payload->source_mac() });

		#ifdef ENABLE_LOGGING
		if (arptable_res.second) {
			LOG("%s | Adding MAC address to ARP table: IP %s ...", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
		}
		#endif
	}

	if (packet.mac_dest() == net::BROADCAST_MAC) 
	{
		LOG("%s | BROADCAST | Resending packet to all available ports...", wire.to.ip().to_string().c_str());
		this->iterate_connections([=](wire_type connection_wire) {
			this->resend(connection_wire, std::move(packet));
		});
		return;
	}

	auto find_res = m_arptable.find(packet.ip_dest());
	if (find_res != m_arptable.end()) 
	{
		LOG("%s | Resending packet to %s ...", wire.to.ip().to_string().c_str(), packet.ip_dest().to_string().c_str());
		this->resend({find_res->second.to, find_res->second.from}, std::move(packet));
		return;
	}
	else {
		throw std::runtime_error("Unknown device");
	}
}
