#pragma once

#include <cstdint>
#include <betterstring/string_view.hpp>
#include <charconv>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <array>

namespace ns {

class ip {
	using array_type = std::uint8_t[4];
public:
	ip(const std::uint8_t A, const std::uint8_t B, const std::uint8_t C, const std::uint8_t D)
		: ip_data{A, B, C, D} {}

	explicit ip(const bs::string_view<> str) {
		std::uint8_t* ip_part = &ip_data[0];
		for (auto part : str.split(".")) {
			(void)std::from_chars(part.data(), part.dataend(), *ip_part++);
		}
	}

	const std::uint8_t* data() const { return ip_data; }
	std::uint8_t* data() { return ip_data; }

	static constexpr std::size_t size() { return 4; }

	const std::uint8_t& operator[](const std::size_t index) const { return ip_data[index]; }
	std::uint8_t& operator[](const std::size_t index) { return ip_data[index]; }

	bool operator==(const ns::ip other) const {
		return std::equal(data(), data() + size(), other.data());
	}
	bool operator!=(const ns::ip other) const { return !(*this == other); }

	bool operator<(const ns::ip other) const {
		return std::lexicographical_compare(data(), data() + size(), other.data(), other.data() + other.size());
	}

private:
	array_type ip_data;
};

inline std::string to_string(const ns::ip ip) {
	std::string result;
	result.reserve(3 * 4 + 3);

	for (std::size_t i = 0; i < ip.size(); ++i) {
		std::array<char, 3> tmp;
		const auto res = std::to_chars(tmp.data(), tmp.data() + tmp.size(), ip[i]);
		result.append(tmp.data(), res.ptr);
		if (i == 3) break;
		result.push_back('.');
	}
	return result;
}

class bad_ip : public std::logic_error {
public:
	bad_ip(const ns::ip& ip) : std::logic_error("invalid ip"), wrong_ip(ip) {}

	const ns::ip& ip() const noexcept { return wrong_ip; }

private:
	ns::ip wrong_ip;
};

}