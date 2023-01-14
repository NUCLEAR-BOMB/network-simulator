#include "Device.hpp"

#include <iostream>

using namespace std::placeholders;

net::Device::Device(CIDR_type cidr) noexcept
	: m_cidr(std::move(cidr)), m_device_port(std::bind(&Device::use_in_packet, this, _1))
{}

void net::Device::add_port(const port_type& other_port) noexcept
{
	m_connetions.emplace_back(other_port);
}

void net::Device::add_connection(Device& other) noexcept {
	this->add_port(other.port());
	other.add_port(this->port());
}

const net::Device::CIDR_type::ip_type& net::Device::ip() const noexcept {
	return m_cidr.ip();
}

const net::Device::CIDR_type::ip_mask_type& net::Device::mask() const noexcept {
	return m_cidr.mask();
}

const net::Device::port_type& net::Device::port() const noexcept {
	return m_device_port;
}

void net::Device::send(const ip_type& dest) const noexcept
{
	net::Packet packet(this->ip(), dest);

	for (const auto& conn : m_connetions) {

		conn.get().send(conn.get(), packet);
	}
}

net::Device::ip_type net::Device::subnet() const noexcept {
	return m_cidr.subnet();
}

bool net::Device::filter_ip(const net::IP& ip) const noexcept
{
	if (ip != this->ip()) return true;

	return false;
}

void net::Device::use_in_packet(const net::Packet& packet) noexcept
{
	if (this->filter_ip(packet.dest())) return;

	std::cout << "source: " << packet.source().to_string() << " dest: " << packet.dest().to_string() << '\n';
}
