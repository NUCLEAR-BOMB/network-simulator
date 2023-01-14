#include "Packet.hpp"

#include <utility>

net::Packet::Packet(ip_type source, ip_type dest) noexcept
	: m_source(std::move(source)), m_destination(std::move(dest))
{}

const net::Packet::ip_type& net::Packet::source() const noexcept {
	return m_source;
}

const net::Packet::ip_type& net::Packet::dest() const noexcept {
	return m_destination;
}

net::Port::Port(CIDR_type cidr, const recive_function_type& func) noexcept
	: m_cidr(std::move(cidr)), m_recive_func(func) {}

void net::Port::send(Port& other, const Packet& packet) const noexcept
{
	if (m_cidr.subnet() != other.m_cidr.subnet()) return;

	other.recive(packet);
}

void net::Port::recive(const Packet& packet) noexcept {
	this->m_recive_func(*this, packet);
}

const net::Port::ip_type& net::Port::ip() const noexcept {
	return m_cidr.ip();
}

const net::Port::ip_mask_type& net::Port::mask() const noexcept {
	return m_cidr.mask();
}

const net::Port::CIDR_type& net::Port::cidr() const noexcept {
	return m_cidr;
}
