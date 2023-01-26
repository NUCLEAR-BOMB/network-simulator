#include "PacketPayloads.hpp"

net::TCP::TCP(tcp_port_type source_port, tcp_port_type dest_port) noexcept
	: m_source_port(source_port)
	, m_dest_port(dest_port)
{}

std::unique_ptr<net::Packet::Payload> net::TCP::clone() const {
	return std::make_unique<TCP>(*this);
}

net::TCP::tcp_port_type net::TCP::source_port() const noexcept {
	return m_source_port;
}

net::TCP::tcp_port_type net::TCP::dest_port() const noexcept {
	return m_dest_port;
}

net::ARP::ARP(Operation oper, const net::MAC& source_mac, const net::MAC& dest_mac) noexcept
	: m_operation_code(oper), m_source_mac(source_mac), m_dest_mac(dest_mac)
{}

std::unique_ptr<net::Packet::Payload> net::ARP::clone() const {
	return std::make_unique<ARP>(*this);
}

net::ARP::Operation net::ARP::operation_code() const noexcept {
	return m_operation_code;
}

const net::MAC& net::ARP::source_mac() const noexcept {
	return m_source_mac;
}

const net::MAC& net::ARP::dest_mac() const noexcept {
	return m_dest_mac;
}
