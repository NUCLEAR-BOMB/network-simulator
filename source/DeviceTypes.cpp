#include "DeviceTypes.hpp"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

void net::Computer::process_packet(const net::Port& in_port, const net::Packet& packet)
{
	const auto* tcp_payload = dynamic_cast<const net::TCP*>(packet.payload());
	if (tcp_payload == nullptr) throw std::runtime_error("Wrong packet payload");

	if (in_port.ip() != packet.dest()) return;

	std::cout 
		<< in_port.ip().to_string() << " - "
		<< "source ip: " << packet.source().to_string() 
		<< "\tdest ip: " << packet.dest().to_string()
		<< "\ttcp port: " << tcp_payload->dest_port()
		<< "\ttcp mac: " << tcp_payload->mac().to_string() << '\n';
}
