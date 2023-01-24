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

	byte_type& raw(std::size_t index);
	const byte_type& raw(std::size_t index) const;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;

	bool operator==(const IP& right) const noexcept;
	bool operator!=(const IP& right) const noexcept;

	bool operator<(const IP& ip) const noexcept;

private:
	array_type m_raw;
};

extern const net::IP BROADCAST_IP;

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

	byte_type& raw(std::size_t index);
	const byte_type& raw(std::size_t index) const;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;

	bool operator==(const MAC& right) const noexcept;
	bool operator!=(const MAC& right) const noexcept;

	static MAC generate() noexcept;

private:
	array_type m_raw;
};

extern const net::MAC BROADCAST_MAC;

class IPMask
{
public:
	using byte_type = std::uint8_t;
	using array_type = byte_type[4];

	using size_type = std::uint_fast8_t;

	static constexpr auto size() noexcept { return std::extent<array_type>::value; }
	static constexpr size_type MAX_CIDR_PREFIX() noexcept { return 32; }

	IPMask() noexcept;
	IPMask(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept;
	explicit IPMask(size_type CIDR_prefix) noexcept;
	IPMask(const std::string& str);
	IPMask(const array_type& other_arr) noexcept;

	byte_type& raw(std::size_t index);
	const byte_type& raw(std::size_t index) const;

	array_type& raw_array() noexcept;
	const array_type& raw_array() const noexcept;

	std::string to_string() const noexcept;
	size_type CIDR_prefix() const noexcept;

	std::uint32_t& to_int() noexcept;
	const std::uint32_t& to_int() const noexcept;

	bool operator==(const IPMask& right) const noexcept;
	bool operator!=(const IPMask& right) const noexcept;

	net::IP operator&(net::IP ip) const noexcept;

private:
	array_type m_raw;
};

class CIDR
{
public:

	using ip_type = IP;
	using ip_mask_type = IPMask;
	  
	CIDR(ip_type ip, ip_mask_type ipmask) noexcept;
	CIDR(const std::string& str);

	ip_type& ip() noexcept;
	ip_mask_type& mask() noexcept;

	const ip_type& ip() const noexcept;
	const ip_mask_type& mask() const noexcept;

	std::string to_string() const noexcept;

	ip_type subnet() const noexcept;

	bool operator==(const CIDR& right) const noexcept;
	bool operator!=(const CIDR& right) const noexcept;

private:
	ip_type m_ip;
	ip_mask_type m_mask;
};

}

