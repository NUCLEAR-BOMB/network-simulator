#include "Device.hpp"

#include <iostream>

net::Device::Device(ip_type ip) noexcept
	: m_ip(std::move(ip)), m_mac(mac_type::generate()), m_device_port(&use_in_packet)
{}

void net::Device::add_port(const port_type& other_port) noexcept
{
	m_connetions.emplace_back(other_port);
}

const net::Device::ip_type& net::Device::ip() const noexcept {
	return m_ip;
}

const net::Device::port_type& net::Device::port() const noexcept {
	return m_device_port;
}

void net::Device::send(const ip_type& dest) const noexcept
{
	net::Packet packet(m_ip, dest);

	for (const auto& conn : m_connetions) {
		conn.get().send(m_device_port, packet);
	}
}

void net::Device::use_in_packet(const net::Packet& packet) noexcept
{
	std::cout << "source: " << packet.source().to_string() << " dest: " << packet.dest().to_string() << '\n';
}
