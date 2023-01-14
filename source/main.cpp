#include "Device.hpp"

int main()
{
	net::Device d1(net::IpAddress("192.168.1.1"));
	net::Device d2(net::IpAddress("192.168.1.2"));

	d1.add_port(d2.port());

	d1.send(d2.ip());

}
