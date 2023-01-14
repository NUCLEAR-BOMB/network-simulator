#include "Device.hpp"

#include <iostream>
#include <cstring>

using namespace std::placeholders;

net::Device::Device() noexcept
	: m_process_in_packet(std::bind(&Device::use_in_packet, this, _1, _2))
{}

net::Device::~Device() noexcept {}

net::Device::port_type net::Device::create_port(CIDR_type device_cidr) const noexcept
{
	return port_type(std::move(device_cidr), m_process_in_packet);
}

void net::Device::add_port(CIDR_type device_cidr, port_type* other_port) noexcept
{
	m_connetions.push_back(
		std::make_pair(
			std::make_unique<port_type>(this->create_port(std::move(device_cidr))),
			other_port
		)
	);
}

void net::Device::add_connection(CIDR_type device_cidr, CIDR_type other_cidr, Device& other) noexcept
{
	auto* device_port = new port_type(this->create_port(device_cidr));
	auto* other_port  = new port_type(other.create_port(other_cidr));

	m_connetions.push_back(std::make_pair(
		std::unique_ptr<port_type>(device_port),
		other_port
	));

	other.m_connetions.push_back(std::make_pair(
		std::unique_ptr<port_type>(other_port),
		device_port
	));
}

void net::Device::send(const ip_type& dest) noexcept
{
	for (auto& conn : m_connetions) 
	{
		conn.first->send(*conn.second, net::Packet(conn.first->ip(), dest));
	}
}

net::Device::ip_type net::Device::subnet(const port_type& port) const noexcept {
	return port.cidr().subnet();
}

const net::Device::port_type& net::Device::port(std::size_t index) const {
	return *m_connetions.at(index).first;
}

bool net::Device::filter_ip(net::Port& in_port, const net::IP& ip) const noexcept
{
	if (ip != in_port.ip()) return true;

	return false;
}

void net::Device::use_in_packet(net::Port& in_port, const net::Packet& packet) noexcept
{
	if (this->filter_ip(in_port, packet.dest())) return;

	std::cout << "source: " << packet.source().to_string() << " dest: " << packet.dest().to_string() << '\n';
}
