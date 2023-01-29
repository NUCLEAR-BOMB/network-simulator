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

net::Interface::Interface(net::CIDR cidr, const recive_function_type& func) noexcept
	: m_recive_func(func) 
	, m_cidr(std::move(cidr))
	, m_mac(net::MAC::generate())
	, m_another(nullptr)
{}

net::Interface::Interface(Interface&& other) noexcept
	: m_recive_func(other.m_recive_func)
	, m_cidr(std::move(other.m_cidr))
	, m_mac(std::move(other.m_mac))
	, m_another(other.m_another)
{
	other.m_another = nullptr;

	if (m_another == nullptr) return;
	if (m_another->m_another != &other) return;

	m_another->m_another = this;
}

net::Interface& net::Interface::operator=(Interface&& other) noexcept
{
	new (this) Interface(std::move(other));
	return *this;
}

net::Interface::~Interface() noexcept
{
	if (m_another == nullptr) return;

	(*m_another).m_another = nullptr;
}

bool net::Interface::send(Packet packet) noexcept
{
	if (m_another == nullptr) return false;

	m_another->m_recive_func(*m_another, std::move(packet));

	return true;

	//if (m_cidr.subnet() != other.m_cidr.subnet()) return;
	//m_another->recive(std::move(packet));
}

void net::Interface::connect_to(Interface* other) noexcept {
	m_another = other;
	other->m_another = this;
}

net::Interface* net::Interface::another() noexcept {
	return m_another;
}

const net::Interface* net::Interface::another() const noexcept {
	return m_another;
}

const net::IP& net::Interface::ip() const noexcept {
	return m_cidr.ip();
}

const net::IPMask& net::Interface::mask() const noexcept {
	return m_cidr.mask();
}

const net::MAC& net::Interface::mac() const noexcept {
	return m_mac;
}

const net::CIDR& net::Interface::cidr() const noexcept {
	return m_cidr;
}

