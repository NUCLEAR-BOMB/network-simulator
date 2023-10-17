#pragma once

#include <client.hpp>
#include <ip.hpp>

#include <betterstring/string_view.hpp>
#include <algorithm>
#include <map>

namespace ns {

class ip_client : public ns::client, public std::enable_shared_from_this<ns::ip_client> {
public:
	explicit ip_client(const ns::ip ip)
		: my_ip(ip) {}

	ns::ip ip() const { return my_ip; }

	void add_connection(std::shared_ptr<ns::ip_client>& conn) {
		connections.insert({conn->ip(), conn});
		conn->connections.insert({ip(), shared_from_this()});
	}
	void send_packet_to(const ns::ip ip, const ns::packet& packet) const {
		try {
			connections.at(ip)->receive(packet);
		} catch (const std::out_of_range&) {
			throw ns::bad_ip(ip);
		}
	}

	void send_message_to(const ns::ip ip, const bs::string_view<> str) const {
		ns::packet packet{str.size()};
		std::memcpy(packet.data(), str.data(), packet.size());
		send_packet_to(ip, packet);
	}

private:
	ns::ip my_ip;
	std::map<ns::ip, std::shared_ptr<ns::ip_client>> connections;
};

}