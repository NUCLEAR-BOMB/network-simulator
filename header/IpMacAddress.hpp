#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <type_traits>

namespace net
{

class IP
{
public:
	using byte_type = std::uint8_t;
	using array_type = byte_type[4];

	static constexpr auto size() noexcept { return std::extent<array_type>::value; }

	IP() noexcept;
	IP(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept;
	IP(const std::string& str);
	IP(const array_type& other_arr) noexcept;

	byte_type& raw(std::size_t index) noexcept;
	const byte_type& raw(std::size_t index) const noexcept;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;

	operator std::string() const noexcept;

	bool operator==(const IP& right) const noexcept;
	bool operator!=(const IP& right) const noexcept;

private:
	array_type m_raw;
};

class MAC
{
public:
	using byte_type = std::uint8_t;
	using array_type = byte_type[6];

	static constexpr auto size() noexcept { return std::extent<array_type>::value; }

	MAC() noexcept;
	MAC(byte_type f1, byte_type f2, byte_type f3, byte_type f4, byte_type f5, byte_type f6) noexcept;
	MAC(const std::string& str);
	MAC(const array_type& other_arr) noexcept;

	byte_type& raw(std::size_t index) noexcept;
	const byte_type& raw(std::size_t index) const noexcept;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;

	bool operator==(const MAC& right) const noexcept;
	bool operator!=(const MAC& right) const noexcept;

	static MAC generate() noexcept;

private:
	array_type m_raw;
};

}

