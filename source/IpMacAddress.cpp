#include "IpMacAddress.hpp"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
#include <stdexcept>
#include <limits>

net::IP::IP() noexcept 
	: m_raw{0}
{}

net::IP::IP(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept
	: m_raw{f1, f2, f3, f4}
{}

net::IP::IP(const std::string& str) 
{
	std::istringstream input(str);
	std::string strnum;

	std::size_t i = 0;
	for (;std::getline(input, strnum, '.') && i < this->size(); ++i) {
		const auto num = std::stoi(strnum);
		if (num < 0 || num > std::numeric_limits<byte_type>::max()) throw std::invalid_argument("Bad input IP string");
		m_raw[i] = static_cast<byte_type>(num);
	}

	if (i < this->size()) throw std::invalid_argument("Bad input IP string");
}

net::IP::IP(const array_type& other_arr) noexcept {
	std::copy(std::begin(other_arr), std::end(other_arr), m_raw);
}

net::IP::byte_type& net::IP::raw(std::size_t index) noexcept {
	return m_raw[index];
}

const net::IP::byte_type& net::IP::raw(std::size_t index) const noexcept {
	return const_cast<IP&>(*this).raw(index);
}

net::IP::array_type& net::IP::raw_array() noexcept {
	return m_raw;
}

const net::IP::array_type& net::IP::raw_array() const noexcept {
	return const_cast<IP&>(*this).raw_array();
}

std::string net::IP::to_string() const noexcept
{
	std::stringstream out;
	for (const auto& val : m_raw) {
		out << std::to_string(val) << '.';
	}
	std::string str = out.str();

	str.pop_back();
	return str;
}

net::IP::operator std::string() const noexcept {
	return this->to_string();
}

bool net::IP::operator==(const IP& right) const noexcept {
	return std::equal(std::begin(m_raw), std::end(m_raw), std::begin(right.m_raw));
}

bool net::IP::operator!=(const IP& right) const noexcept {
	return !(*this == right);
}

net::MAC::MAC() noexcept 
	: m_raw{0}
{}

net::MAC::MAC(byte_type f1, byte_type f2, byte_type f3, byte_type f4, byte_type f5, byte_type f6) noexcept
	: m_raw{f1, f2, f3, f4, f5, f6}
{}

net::MAC::MAC(const std::string& str) 
{
	std::istringstream input(str);
	std::string strnum;

	for (std::size_t i = 0; std::getline(input, strnum, ':'), i < this->size(); ++i) {
		m_raw[i] = static_cast<byte_type>(std::stoi(strnum, nullptr, 16));
	}
}

net::MAC::MAC(const array_type& other_arr) noexcept {
	std::copy(std::begin(other_arr), std::end(other_arr), std::begin(m_raw));
}

net::MAC::byte_type& net::MAC::raw(std::size_t index) noexcept {
	return m_raw[index];
}

const net::MAC::byte_type& net::MAC::raw(std::size_t index) const noexcept {
	return const_cast<MAC&>(*this).raw(index);
}

net::MAC::array_type& net::MAC::raw_array() noexcept {
	return m_raw;
}

const net::MAC::array_type& net::MAC::raw_array() const noexcept {
	return const_cast<MAC&>(*this).raw_array();
}

std::string net::MAC::to_string() const noexcept {
	std::stringstream out;

	for (const auto& val : m_raw) {
		out << std::hex << val << ':';
	}
	out.seekp(-1, std::ios_base::end);
	return out.str();
}

bool net::MAC::operator==(const MAC& right) const noexcept {
	return std::equal(std::begin(m_raw), std::end(m_raw), std::begin(right.m_raw));
}

bool net::MAC::operator!=(const MAC& right) const noexcept {
	return !(*this == right);
}

net::MAC net::MAC::generate() noexcept
{
	using rand_engine_type = std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned int>;

	rand_engine_type rand_engine;

	MAC v;

	std::generate(std::begin(v.raw_array()), std::end(v.raw_array()), [&]() {
		return static_cast<byte_type>(rand_engine());
	});

	return v;
}
