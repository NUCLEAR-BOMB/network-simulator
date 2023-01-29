#pragma once

#include "Packet.hpp"

#include <map>
#include <deque>
#include <utility>

namespace net
{

class ARPTable
{
public:

	using container_type = std::map<net::IP, net::MAC>;

	const net::MAC* find(const net::IP& ip) const noexcept;

	bool add_back(net::IP ip, net::MAC mac) noexcept;

private:
	container_type m_container;
};

class RoutingTable
{
public:

	struct container_value_type 
	{
		net::IP ip;
		net::IPMask mask;

		net::Interface& interface;
	};

	using container_type = std::deque<container_value_type>;

	container_value_type& find(const net::IP& ip);

	void add_back(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept;

	void add_front(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept;

private:

	container_type m_container;
};

}