#pragma once

#include <memory>
#include <cstdint>
#include <utility>
#include <algorithm>

namespace ns {

class packet {
public:
	packet() : packet_data() {}

	explicit packet(const std::size_t size) 
		: packet_data(new std::uint8_t[sizeof(std::size_t) + size]) {
		get_size() = size;
	}
	template<std::size_t N>
	explicit packet(const std::uint8_t(&bytes)[N]) : packet(N) {
		std::memcpy(data(), bytes, N);
	}

	packet(const packet&) = default;
	packet(packet&&) = default;

	packet& operator=(const packet&) = default;
	packet& operator=(packet&&) = default;

	void* data() { return data_u8(); }
	const void* data() const { return data_u8(); }

	std::uint8_t& operator[](const std::size_t index) {
		// TODO: add a check for index out of bounds
		return data_u8()[index];
	}

	std::uint8_t operator[](const std::size_t index) const {
		// TODO: add a check for index out of bounds
		return data_u8()[index];
	}

	packet copy() const {
		packet result(size());
		std::copy_n(data_u8(), size(), result.data_u8());
		return result;
	}

	const std::uint8_t* data_u8() const { return packet_data.get() + sizeof(std::size_t); }
	std::uint8_t* data_u8() { return const_cast<std::uint8_t*>(std::as_const(*this).data_u8()); }

	std::size_t size() const { return get_size(); }

	bool operator==(const ns::packet& other) const {
		if (size() != other.size()) return false;
		return std::equal(data_u8(), data_u8() + size(), other.data_u8());
	}
	bool operator!=(const ns::packet& other) const { return !(*this == other); }

private:
	const std::size_t& get_size() const { return *reinterpret_cast<std::size_t*>(packet_data.get() + 0); }
	std::size_t& get_size() { return const_cast<std::size_t&>(std::as_const(*this).get_size()); }

	std::shared_ptr<std::uint8_t[]> packet_data;
};

}
