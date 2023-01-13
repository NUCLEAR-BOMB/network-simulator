#include "IpMacAddress.hpp"

#include <algorithm>
#include <sstream>
#include <iomanip>

net::IpAddress::IpAddress(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept
	: m_raw{f1, f2, f3, f4}
{}

net::IpAddress::IpAddress(const std::string& str) 
{
	std::istringstream input(str);
	std::string strnum;

	for (std::size_t i = 0; std::getline(input, strnum, '.'), i < this->size(); ++i) {
		m_raw[i] = static_cast<byte_type>(std::stoi(strnum));
	}
}

net::IpAddress::IpAddress(const array_type& other_arr) noexcept {
	std::copy(std::begin(other_arr), std::end(other_arr), m_raw);
}

net::IpAddress::byte_type& net::IpAddress::raw(std::size_t index) noexcept {
	return m_raw[index];
}

const net::IpAddress::byte_type& net::IpAddress::raw(std::size_t index) const noexcept {
	return const_cast<IpAddress&>(*this).raw(index);
}

net::IpAddress::array_type& net::IpAddress::raw_array() noexcept {
	return m_raw;
}

const net::IpAddress::array_type& net::IpAddress::raw_array() const noexcept {
	return const_cast<IpAddress&>(*this).raw_array();
}

std::string net::IpAddress::to_string() const noexcept
{
	std::stringstream out;
	for (const auto& val : m_raw) {
		out << val << '.';
	}
	out.seekp(-1, std::ios_base::end);
	return out.str();
}

bool net::IpAddress::operator==(const IpAddress& right) const noexcept {
	return std::equal(std::begin(m_raw), std::end(m_raw), std::begin(right.m_raw));
}

bool net::IpAddress::operator!=(const IpAddress& right) const noexcept {
	return !(*this == right);
}

net::MacAddress::MacAddress(byte_type f1, byte_type f2, byte_type f3, byte_type f4, byte_type f5, byte_type f6) noexcept
	: m_raw{f1, f2, f3, f4, f5, f6}
{}

net::MacAddress::MacAddress(const std::string& str) 
{
	std::istringstream input(str);
	std::string strnum;

	for (std::size_t i = 0; std::getline(input, strnum, ':'), i < this->size(); ++i) {
		m_raw[i] = static_cast<byte_type>(std::stoi(strnum, nullptr, 16));
	}
}

net::MacAddress::MacAddress(const array_type& other_arr) noexcept {
	std::copy(std::begin(other_arr), std::end(other_arr), std::begin(m_raw));
}

net::MacAddress::byte_type& net::MacAddress::raw(std::size_t index) noexcept {
	return m_raw[index];
}

const net::MacAddress::byte_type& net::MacAddress::raw(std::size_t index) const noexcept {
	return const_cast<MacAddress&>(*this).raw(index);
}

net::MacAddress::array_type& net::MacAddress::raw_array() noexcept {
	return m_raw;
}

const net::MacAddress::array_type& net::MacAddress::raw_array() const noexcept {
	return const_cast<MacAddress&>(*this).raw_array();
}

std::string net::MacAddress::to_string() const noexcept {
	std::stringstream out;

	for (const auto& val : m_raw) {
		out << std::hex << val << ':';
	}
	out.seekp(-1, std::ios_base::end);
	return out.str();
}

bool net::MacAddress::operator==(const MacAddress& right) const noexcept {
	return std::equal(std::begin(m_raw), std::end(m_raw), std::begin(right.m_raw));
}

bool net::MacAddress::operator!=(const MacAddress& right) const noexcept {
	return !(*this == right);
}
