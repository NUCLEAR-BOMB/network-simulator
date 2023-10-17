#pragma once

#include <cstdlib>
#include <array>

#include <packet.hpp>

namespace ns {

class packet_pull {
	static constexpr std::size_t max_size = 50;
public:
	std::size_t size() const { return pull_size; }

	void push(const ns::packet& pac) {
		// TODO: add a check for index out of bounds
		pull_array[pull_size] = pac;
		++pull_size;
	}
	const ns::packet& last() const {
		return pull_array[pull_size - 1];
	}

	ns::packet get_and_pop() {
		pull_size -= 1;
		return std::move(pull_array[pull_size]);
	}

	const ns::packet& operator[](const std::size_t index) const {
		// TODO: add a check for index out of bounds
		return pull_array[index];
	}

	auto begin() const { return pull_array.begin(); }
	auto end() const { return pull_array.end(); }

private:
	std::array<ns::packet, max_size> pull_array = {};
	std::size_t pull_size = 0;
};

}