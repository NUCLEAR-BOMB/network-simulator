#include "Device.hpp"

#include <iostream>

int main()
{
	using net::IP;
	using net::IPMask;
	using net::Port;

	net::Device d1;
	net::Device d2;

	d1.add_connection(
		net::CIDR("192.168.1.100/24"),
		net::CIDR("192.168.1.250/24"),
	d2);

	const auto& port0 = d1.port(0);
	std::cout << "this ip: " << port0.ip().to_string() << " this ip mask: " << port0.mask().to_string() << '\n';

	std::string dest_ip;
	while (std::cin >> dest_ip)
	{
		d1.send(IP(dest_ip));
	}
}
