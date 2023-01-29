#pragma once

#include "Device.hpp"

namespace net
{

class Computer : public net::Device
{
protected:

	virtual void process_packet(net::Interface& wire, net::Packet packet) override;

private:

};

class Switch : public net::Device
{
protected:

	virtual void process_packet(net::Interface& wire, net::Packet packet) override;

};



}
