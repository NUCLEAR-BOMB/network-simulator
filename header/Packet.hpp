#pragma once

#include "NetworkStructures.hpp"

#include <functional>
#include <vector>
#include <memory>
#include <utility>

namespace net
{

class Packet
{
public:
	using byte_type = std::uint8_t;
	using ip_type = net::IP;

	struct Payload {
		virtual ~Payload() {};
	};

	Packet(ip_type source, ip_type dest) noexcept;

	Packet(ip_type source, ip_type dest, std::unique_ptr<Payload>&& payload) noexcept;

	const ip_type& source() const noexcept;
	const ip_type& dest() const noexcept;

	Payload* payload() noexcept;
	const Payload* payload() const noexcept;

	struct header_type 
	{
		ip_type source;
		ip_type dest;
	};

private:
	header_type m_header;

	std::unique_ptr<Payload> m_payload;
};

class Port
{
public:

	using recived_port = Port&;
	using sended_port = Port&;

	using recive_function_type = std::function<void(recived_port, sended_port, const Packet&)>;

	using CIDR_type = net::CIDR;
	using MAC_type = net::MAC;

	using ip_type = typename net::CIDR::ip_type;
	using ip_mask_type = typename net::CIDR::ip_mask_type;

	Port(CIDR_type cidr, const recive_function_type& func) noexcept;

	void send(recived_port other, const Packet& packet) noexcept;
	void recive(sended_port from, const Packet& packet) noexcept;

	const ip_type& ip() const noexcept;
	const ip_mask_type& mask() const noexcept;
	const MAC_type& mac() const noexcept;

	const CIDR_type& cidr() const noexcept;

private:
	const recive_function_type& m_recive_func;
	CIDR_type m_cidr;
	const MAC_type m_mac;
};

}