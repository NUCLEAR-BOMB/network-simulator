#pragma once

#include "IpMacAddress.hpp"

#include <functional>

namespace net
{

class Packet
{
public:
	using ip_type = net::IpAddress;

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
	using recive_function_type = std::function<void(const Packet&)>;

	Port(recive_function_type func) noexcept;

	void send(const Port& other, const Packet& packet) const noexcept;
	void recive(const Packet& packet) const noexcept;

private:
	recive_function_type m_recive_func;
};

}