#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <type_traits>

namespace net
{

class IpAddress
{
public:
	using byte_type = std::uint8_t;
	using array_type = byte_type[4];

	static constexpr auto size() noexcept { return std::extent<array_type>::value; }

	IpAddress() noexcept;
	IpAddress(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept;
	IpAddress(const std::string& str);
	IpAddress(const array_type& other_arr) noexcept;

	byte_type& raw(std::size_t index) noexcept;
	const byte_type& raw(std::size_t index) const noexcept;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;

	bool operator==(const IpAddress& right) const noexcept;
	bool operator!=(const IpAddress& right) const noexcept;

private:
	array_type m_raw;
};

class MacAddress
{
public:
	using byte_type = std::uint8_t;
	using array_type = byte_type[6];

	static constexpr auto size() noexcept { return std::extent<array_type>::value; }

	MacAddress() noexcept;
	MacAddress(byte_type f1, byte_type f2, byte_type f3, byte_type f4, byte_type f5, byte_type f6) noexcept;
	MacAddress(const std::string& str);
	MacAddress(const array_type& other_arr) noexcept;

	byte_type& raw(std::size_t index) noexcept;
	const byte_type& raw(std::size_t index) const noexcept;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;

	bool operator==(const MacAddress& right) const noexcept;
	bool operator!=(const MacAddress& right) const noexcept;

	static MacAddress generate() noexcept;

private:
	array_type m_raw;
};

}

