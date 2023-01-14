#include "Device.hpp"

#include <iostream>

int main()
{
	using net::IP;

	net::Device d1(net::IP("192.168.1.1"));
	net::Device d2(net::IP("192.168.1.2"));

	d1.add_connection(d2);

	//d1.send(d2.ip());

	std::cout << "this ip: " << d2.ip().to_string() << '\n';

	std::string dest_ip;
	while (std::cin >> dest_ip)
	{
		d2.send(IP(dest_ip));
	}
}
