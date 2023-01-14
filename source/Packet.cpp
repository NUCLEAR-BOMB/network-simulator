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

net::Port::Port(recive_function_type func) noexcept 
	: m_recive_func(std::move(func)) {}

void net::Port::send(const Port& other, const Packet& packet) const noexcept {
	other.recive(packet);
}

void net::Port::recive(const Packet& packet) const noexcept {
	this->m_recive_func(packet);
}
