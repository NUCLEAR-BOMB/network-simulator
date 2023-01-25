#pragma once

#include "Device.hpp"

namespace net
{

class Computer : public net::Device
{
protected:

	virtual void process_packet(wire_type wire, const net::Packet& packet) override;

private:

};

class Switch : public net::Device
{
protected:

	void resend(wire_type wire, const net::Packet& packet);

	virtual void process_packet(wire_type wire, const net::Packet& packet) override;

};



}
