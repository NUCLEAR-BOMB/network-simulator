#include "Logging.hpp"
#include "DeviceTypes.hpp"

#include <iostream>

int main()
{
	net::Computer d1({192,168,1,100});
	net::Computer d2({192,168,1,101});
	net::Computer d3({192,168,1,102});
	net::Computer d4({192,168,1,103});

	net::Switch sw1({192,168,1,90});
	net::Switch sw2({192,168,1,80});

	d1.connect_to(sw1, {192,168,1,2});
	d2.connect_to(sw1, {192,168,1,3});

	sw1.connect_to(sw2);

	d3.connect_to(sw2, {192,168,1,6});
	d4.connect_to(sw2, {192,168,1,7});

	const auto& port0 = d1.port(0);
	std::cout << "this ip: " << port0.ip().to_string() << " this ip mask: " << port0.mask().to_string() << '\n';

	std::string dest_ip;
	while (std::cin >> dest_ip)
	{
		d1.send({ dest_ip });
	}
}
