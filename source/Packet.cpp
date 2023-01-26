#include "Packet.hpp"

#include "PacketPayloads.hpp"

#include <utility>

net::Packet::Packet(const net::MAC& source, const net::MAC& dest, const net::IP& ip_source, const net::IP& ip_dest) noexcept
	: m_header{source, dest}
	, m_ip_header{ip_source, ip_dest}
	, m_payload(nullptr)
{}

net::Packet::Packet(const net::MAC& source, const net::MAC& dest, const net::IP& ip_source, const net::IP& ip_dest, std::unique_ptr<Payload>&& payload) noexcept
	: m_header{ source, dest }
	, m_ip_header{ip_source, ip_dest}
	, m_payload(std::move(payload))
{}

net::Packet::Packet(const Packet& other) noexcept
	: m_header(other.m_header)
	, m_ip_header(other.m_ip_header)
	, m_payload(other.m_payload->clone())
{}

const net::MAC& net::Packet::mac_source() const noexcept {
	return m_header.source;
}

const net::MAC& net::Packet::mac_dest() const noexcept {
	return m_header.dest;
}

const net::IP& net::Packet::ip_source() const noexcept {
	return m_ip_header.source;
}

const net::IP& net::Packet::ip_dest() const noexcept {
	return m_ip_header.dest;
}

net::Packet::Payload* net::Packet::payload() noexcept {
	return m_payload.get();
}

const net::Packet::Payload* net::Packet::payload() const noexcept {
	return m_payload.get();
}

net::Interface::Interface(CIDR_type cidr, const recive_function_type& func) noexcept
	: m_recive_func(func) 
	, m_cidr(std::move(cidr))
	, m_mac(MAC_type::generate())
{}

void net::Interface::send(recived_port other, Packet packet) noexcept
{
	//if (m_cidr.subnet() != other.m_cidr.subnet()) return;

	other.recive(*this, std::move(packet));
}

void net::Interface::recive(sended_port from, Packet packet) noexcept {
	this->m_recive_func(*this, from, std::move(packet));
}

const net::Interface::ip_type& net::Interface::ip() const noexcept {
	return m_cidr.ip();
}

const net::Interface::ip_mask_type& net::Interface::mask() const noexcept {
	return m_cidr.mask();
}

const net::Interface::MAC_type& net::Interface::mac() const noexcept {
	return m_mac;
}

const net::Interface::CIDR_type& net::Interface::cidr() const noexcept {
	return m_cidr;
}
