#include "DeviceTypes.hpp"

#include "Logging.hpp"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

void net::Computer::process_packet(net::Interface& interface, net::Packet packet)
{
	const auto* tcp_payload = dynamic_cast<const net::TCP*>(packet.payload());
	if (tcp_payload == nullptr) {
		LOG("%s | Ignoring non-TCP packet", interface.ip().to_string().c_str());
		return;
	}

	if (!this->verify_in_packet(interface, packet)) {
		LOG("%s | Ignoring wrong destination IP address: %s", interface.ip().to_string().c_str(), packet.ip_dest().to_string().c_str());
		return;
	}

	LOG("%s | Get packet!", interface.ip().to_string().c_str());

	std::cout
		<< '\n'
		<< interface.ip().to_string()		<< " - "
		<< "\n   source ip: \t\t"		<< packet.ip_source().to_string()
		<< "\n   dest ip: \t\t"			<< packet.ip_dest().to_string()
		<< "\n   tcp source port: \t"	<< tcp_payload->source_port()
		<< "\n   tcp dest port: \t"		<< tcp_payload->dest_port() << "\n\n";
}

void net::Switch::process_packet(net::Interface& interface, net::Packet packet)
{
	LOG("%s | Forwarding packet from %s", interface.ip().to_string().c_str(), packet.ip_source().to_string().c_str());

	if (interface.cidr().subnet() != (interface.mask() & packet.ip_source())) return;

	const auto* arp_payload = dynamic_cast<const net::ARP*>(packet.payload());

	if (arp_payload != nullptr) 
	{
		LOG("%s | Adding MAC address to Switch ARP table: IP %s", interface.ip().to_string().c_str(), packet.ip_source().to_string().c_str());
		m_arptable.add_back(packet.ip_source(), arp_payload->source_mac());

		LOG("%s | Adding interface to Switch Routing table", interface.ip().to_string().c_str());
		m_routingtable.add_back(packet.ip_source(), net::IPMask{255,255,255,255}, interface);
	}

	if (packet.mac_dest() == net::BROADCAST_MAC) 
	{
		//LOG("%s | BROADCAST | Resending packet to all available ports...", wire.to.ip().to_string().c_str());
		
		for (auto& conn : m_connetions)
		{
			if (&conn == &interface) continue;
			if (conn.cidr().subnet() != (conn.mask() & packet.ip_dest())) continue;

			LOG("%s | Resending packet to an interface %s", interface.ip().to_string().c_str(),
				conn.another()->ip().to_string().c_str()
			);

			conn.send(packet); // copy
		}
		return;
	}

	//this->resend(find_res->second.wire, std::move(packet));

	//LOG("%s | Resending packet to %s", wire.to.ip().to_string().c_str(), packet.ip_dest().to_string().c_str());

	this->send_packet(packet.ip_dest(), std::move(packet));

	//this->send_payload(packet.ip_dest(), std::move(packet));
}
