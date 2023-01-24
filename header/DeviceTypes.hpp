#pragma once

#include "Device.hpp"

namespace net
{

class Computer : public net::Device
{
public:

	virtual void process_packet(const net::Port& in_port, const net::Packet& packet) override;

private:

};

}
