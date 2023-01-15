#include "Device.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std::placeholders;

net::Device::Device() noexcept
	: m_process_in_packet(std::bind(&Device::process_packet, this, _1, _2))
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
	for (auto conn = m_connetions.begin(); conn != m_connetions.end();) 
	{
		if (std::shared_ptr<port_type> conn_second = conn->second.lock()) {
			conn->first->send(*conn_second, 
				net::Packet(
					conn->first->ip(), dest, 
					net::TCP(1234, conn->first->mac())
				)
			);
		} else {
			conn = m_connetions.erase(conn); continue;
		}
		++conn;
	}
}

net::Device::ip_type net::Device::subnet(const port_type& port) const noexcept {
	return port.cidr().subnet();
}

const net::Device::port_type& net::Device::port(std::size_t index) const {
	return *m_connetions.at(index).first;
}


void net::Device::process_packet(net::Port& in_port, const net::Packet& packet)
{
	const auto* tcp_payload = dynamic_cast<const TCP*>(packet.payload());
	if (tcp_payload == nullptr) throw std::runtime_error("Wrong packet payload type");

	if (packet.dest() != in_port.ip()) return;

	std::cout 
		<< "source: " << packet.source().to_string()
		<< "\tdest: " << packet.dest().to_string()
		<< "\tport: " << tcp_payload->port() << '\n';
}
