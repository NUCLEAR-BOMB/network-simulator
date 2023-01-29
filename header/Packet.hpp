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

class Interface
{
public:

	using recive_function_type = std::function<void(Interface&, Packet)>;

	Interface(net::CIDR cidr, const recive_function_type& func) noexcept;

	Interface(const Interface&) noexcept = delete;
	Interface& operator=(const Interface&) noexcept = delete;

	Interface(Interface&& other) noexcept;
	Interface& operator=(Interface&& other) noexcept;

	~Interface() noexcept;

	bool send(Packet packet) noexcept;

	void connect_to(Interface* other) noexcept;

	Interface* another() noexcept;
	const Interface* another() const noexcept;

	const net::IP& ip() const noexcept;
	const net::IPMask& mask() const noexcept;
	const net::MAC& mac() const noexcept;

	const net::CIDR& cidr() const noexcept;

private:
	const recive_function_type& m_recive_func;
	net::CIDR m_cidr;
	net::MAC m_mac;

	Interface* m_another;
};

}