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

	struct Payload {
		virtual ~Payload() {};
		virtual std::unique_ptr<Payload> clone() const = 0;
	};

	Packet(const net::MAC& source, const net::MAC& dest, const net::IP& ip_source, const net::IP& ip_dest) noexcept;

	Packet(const net::MAC& source, const net::MAC& dest, const net::IP& ip_source, const net::IP& ip_dest, std::unique_ptr<Payload>&& payload) noexcept;

	Packet(const Packet& other) noexcept;
	Packet(Packet&&) noexcept = default;

	const net::MAC& mac_source() const noexcept;
	const net::MAC& mac_dest() const noexcept;

	const net::IP& ip_source() const noexcept;
	const net::IP& ip_dest() const noexcept;

	Payload* payload() noexcept;
	const Payload* payload() const noexcept;

	struct IP_header 
	{
		net::IP source;
		net::IP dest;
	};

	struct MAC_header 
	{
		net::MAC source;
		net::MAC dest;
	};

private:
	MAC_header m_header;
	IP_header m_ip_header;

	std::unique_ptr<Payload> m_payload;
};

class Port
{
public:

	using recived_port = Port&;
	using sended_port = Port&;

	using recive_function_type = std::function<void(recived_port, sended_port, Packet)>;

	using CIDR_type = net::CIDR;
	using MAC_type = net::MAC;

	using ip_type = typename net::CIDR::ip_type;
	using ip_mask_type = typename net::CIDR::ip_mask_type;

	Port(CIDR_type cidr, const recive_function_type& func) noexcept;

	void send(recived_port other, Packet packet) noexcept;
	void recive(sended_port from, Packet packet) noexcept;

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