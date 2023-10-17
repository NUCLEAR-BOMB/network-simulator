#pragma once

#include <packet_pull.hpp>

namespace ns {

class client {
public:
	const ns::packet_pull& received() const { return received_packets; }
protected:
	void receive(const ns::packet& pac) {
		received_packets.push(pac);
	}

	ns::packet_pull received_packets;
};

}

