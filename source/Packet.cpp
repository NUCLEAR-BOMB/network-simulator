#include "Packet.hpp"

#include <utility>

net::Packet::Packet(ip_type source, ip_type dest) noexcept
	: m_header{std::move(source), std::move(dest)}
	, m_payload(nullptr)
{}

net::Packet::Packet(ip_type source, ip_type dest, std::unique_ptr<Payload>&& payload) noexcept
	: m_header{ std::move(source), std::move(dest) }
	, m_payload(std::move(payload))
{}

const net::Packet::ip_type& net::Packet::source() const noexcept {
	return m_header.source;
}

const net::Packet::ip_type& net::Packet::dest() const noexcept {
	return m_header.dest;
}

net::Packet::Payload* net::Packet::payload() noexcept {
	return m_payload.get();
}

const net::Packet::Payload* net::Packet::payload() const noexcept {
	return m_payload.get();
}

net::Port::Port(CIDR_type cidr, const recive_function_type& func) noexcept
	: m_recive_func(func) 
	, m_cidr(std::move(cidr))
	, m_mac(MAC_type::generate())
{}

void net::Port::send(recived_port other, const Packet& packet) noexcept
{
	if (m_cidr.subnet() != other.m_cidr.subnet()) return;

	other.recive(*this, packet);
}

void net::Port::recive(sended_port from, const Packet& packet) noexcept {
	this->m_recive_func(*this, from, packet);
}

const net::Port::ip_type& net::Port::ip() const noexcept {
	return m_cidr.ip();
}

const net::Port::ip_mask_type& net::Port::mask() const noexcept {
	return m_cidr.mask();
}

const net::Port::MAC_type& net::Port::mac() const noexcept {
	return m_mac;
}

const net::Port::CIDR_type& net::Port::cidr() const noexcept {
	return m_cidr;
}
