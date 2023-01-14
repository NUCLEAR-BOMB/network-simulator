#pragma once

#include "NetworkStructures.hpp"

#include <functional>

namespace net
{

class Packet
{
public:
	using ip_type = net::IP;

	Packet(ip_type source, ip_type dest) noexcept;

	const ip_type& source() const noexcept;
	const ip_type& dest() const noexcept;

private:
	ip_type m_source;
	ip_type m_destination;
};

class Port
{
public:
	using recive_function_type = std::function<void(Port&, const Packet&)>;

	using CIDR_type = net::CIDR;

	using ip_type = typename net::CIDR::ip_type;
	using ip_mask_type = typename net::CIDR::ip_mask_type;

	Port(CIDR_type cidr, const recive_function_type& func) noexcept;

	void send(Port& other, const Packet& packet) const noexcept;
	void recive(const Packet& packet) noexcept;

	const ip_type& ip() const noexcept;
	const ip_mask_type& mask() const noexcept;

	const CIDR_type& cidr() const noexcept;

private:
	const recive_function_type& m_recive_func;
	CIDR_type m_cidr;
};

}