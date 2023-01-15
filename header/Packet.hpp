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

	template<class Derived>
	Packet(ip_type source, ip_type dest, Derived&& payload) noexcept
		: Packet(std::move(source), std::move(dest)) {
		m_payload = std::make_shared<Derived>(std::forward<Derived>(payload));
	}

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

	std::shared_ptr<Payload> m_payload;
};

class Port
{
public:
	using recive_function_type = std::function<void(Port&, const Packet&)>;

	using CIDR_type = net::CIDR;
	using MAC_type = net::MAC;

	using ip_type = typename net::CIDR::ip_type;
	using ip_mask_type = typename net::CIDR::ip_mask_type;

	Port(CIDR_type cidr, const recive_function_type& func) noexcept;

	void send(Port& other, const Packet& packet) const noexcept;
	void recive(const Packet& packet) noexcept;

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