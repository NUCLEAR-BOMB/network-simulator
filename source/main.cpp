#include "Logging.hpp"
#include "DeviceTypes.hpp"

#include <iostream>

int main()
{
	using net::IP;
	using net::IPMask;
	using net::Interface;

	net::Computer d1;
	net::Computer d2;
	net::Computer d3;
	net::Computer d4;

	net::Switch sw1;
	net::Switch sw2;

	d1.add_connection(sw1,
		{"192.168.1.100"},
		{"192.168.1.2"}
	);
	d2.add_connection(sw1,
		{"192.168.1.101"},
		{"192.168.1.3"}
	);

	sw1.add_connection(sw2,
		{"192.168.1.4"},
		{"192.168.1.5"}
	);

	d3.add_connection(sw2,
		{"192.168.1.102"},
		{"192.168.1.6"}
	);
	d4.add_connection(sw2,
		{"192.168.1.103"},
		{"192.168.1.7"}
	);

	const auto& port0 = d1.port(0);
	std::cout << "this ip: " << port0.ip().to_string() << " this ip mask: " << port0.mask().to_string() << '\n';

	std::string dest_ip;
	while (std::cin >> dest_ip)
	{
		d1.send({ dest_ip });
	}
}
