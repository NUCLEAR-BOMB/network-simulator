#pragma once

#include "Packet.hpp"

namespace net
{

class TCP : public net::Packet::Payload
{
public:
	using tcp_port_type = unsigned int;

	TCP(tcp_port_type source_port, tcp_port_type dest_port) noexcept;

	virtual std::unique_ptr<Payload> clone() const override;

	tcp_port_type source_port() const noexcept;
	tcp_port_type dest_port() const noexcept;

private:
	tcp_port_type m_source_port;
	tcp_port_type m_dest_port;
};

class ARP : public net::Packet::Payload
{
public:
	enum class Operation : std::uint16_t {
		Request = 0x1,
		Reply	= 0x2,
	};

	ARP(Operation oper, const net::MAC& source_mac, const net::MAC& dest_mac) noexcept;

	virtual std::unique_ptr<Payload> clone() const override;

	Operation operation_code() const noexcept;

	const net::MAC& source_mac() const noexcept;
	const net::MAC& dest_mac() const noexcept;

private:
	Operation m_operation_code;
	net::MAC m_source_mac;
	net::MAC m_dest_mac;
};

}