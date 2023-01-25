#include "DeviceTypes.hpp"

#include <iostream>

int main()
{
	using net::IP;
	using net::IPMask;
	using net::Port;

	net::Computer d1;
	net::Computer d2;

	net::Switch sw;

	d1.add_connection(sw,
		net::CIDR("192.168.1.100/24"),
		net::CIDR("192.168.1.2/24")
	);
	d2.add_connection(sw,
		net::CIDR("192.168.1.200/24"),
		net::CIDR("192.168.1.3/24")
	);

	const auto& port0 = d1.port(0);
	std::cout << "this ip: " << port0.ip().to_string() << " this ip mask: " << port0.mask().to_string() << '\n';

	std::string dest_ip;
	while (std::cin >> dest_ip)
	{
		d1.send(IP(dest_ip));
	}
}
