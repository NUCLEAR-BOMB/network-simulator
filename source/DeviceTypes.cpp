#include "DeviceTypes.hpp"

#include "Logging.hpp"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

void net::Computer::process_packet(wire_type wire, net::Packet packet)
{
	const auto* tcp_payload = dynamic_cast<const net::TCP*>(packet.payload());
	if (tcp_payload == nullptr) {
		LOG("%s | Ignoring non-TCP packet", wire.to.ip().to_string().c_str()); 
		return;
	}

	if (!this->verify_in_packet(wire, packet)) {
		LOG("%s | Ignoring wrong destination IP address: %s", wire.to.ip().to_string().c_str(), packet.ip_dest().to_string().c_str());
		return;
	}

	LOG("%s | Get packet!", wire.to.ip().to_string().c_str());

	std::cout
		<< wire.to.ip().to_string()		<< " - "
		<< "\n   source ip: \t\t"		<< packet.ip_source().to_string()
		<< "\n   dest ip: \t\t"			<< packet.ip_dest().to_string()
		<< "\n   tcp source port: \t"	<< tcp_payload->source_port()
		<< "\n   tcp dest port: \t"		<< tcp_payload->dest_port() << '\n';
}


void net::Switch::resend(wire_type wire, net::Packet packet) const
{
	LOG("%s | Resending packet from %s to %s", wire.to.ip().to_string().c_str(),
		packet.ip_source().to_string().c_str(),
		wire.from.ip().to_string().c_str()
	);
	wire.to.send(wire.from, std::move(packet));
}

void net::Switch::process_packet(wire_type wire, net::Packet packet)
{
	LOG("%s | Forwarding packet from %s", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());

	if (wire.to.cidr().subnet() != (wire.to.mask() & packet.ip_source())) return;

	const auto* arp_payload = dynamic_cast<const net::ARP*>(packet.payload());

	if (arp_payload != nullptr) 
	{
		#ifdef ENABLE_LOGGING
		auto arptable_res =
		#endif
		m_arptable.emplace(packet.ip_source(), arptable_mapped_t{ wire, arp_payload->source_mac() });

		#ifdef ENABLE_LOGGING
		if (arptable_res.second) {
			LOG("%s | Adding MAC address to ARP table: IP %s", wire.to.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
		}
		#endif
	}

	if (packet.mac_dest() == net::BROADCAST_MAC) 
	{
		//LOG("%s | BROADCAST | Resending packet to all available ports...", wire.to.ip().to_string().c_str());
		this->iterate_connections([&](wire_type conn_wire)
		{
			if (conn_wire.to.cidr().subnet() != (conn_wire.to.mask() & packet.ip_dest())) return;

			this->resend(conn_wire, packet);
		});
		return;
	}

	auto find_res = m_arptable.find(packet.ip_dest());
	if (find_res != m_arptable.end()) 
	{
		//LOG("%s | Resending packet to %s", wire.to.ip().to_string().c_str(), packet.ip_dest().to_string().c_str());
		this->resend(find_res->second.wire, std::move(packet));
		return;
	}
	else {
		throw std::runtime_error("Unknown device");
	}
}
