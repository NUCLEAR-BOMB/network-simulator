#include "NetworkStructures.hpp"

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>
#include <functional>
#include <stdexcept>
#include <limits>

const net::IP BROADCAST_IP = net::IP("0.0.0.0");

namespace {

static void string_to_arr4(std::uint8_t(&raw)[4], const std::string& str)
{
	using arr_sz = std::extent<std::remove_reference_t<decltype(raw)>>;
	using arr_type = std::remove_reference_t<decltype(raw[0])>;

	std::istringstream input(str);
	std::string strnum;

	std::size_t i = 0;
	for (; std::getline(input, strnum, '.') && i < arr_sz(); ++i) {
		const auto num = std::stoi(strnum);
		if (num < 0 || num > std::numeric_limits<arr_type>::max()) throw std::invalid_argument("Bad input IP string");
		raw[i] = static_cast<arr_type>(num);
	}

	if (i < arr_sz()) throw std::invalid_argument("Bad input IP string");
}

static std::string arr4_to_string(const std::uint8_t(&raw)[4])
{
	std::stringstream out;
	for (const auto& val : raw) {
		out << std::to_string(val) << '.';
	}
	std::string str = out.str();

	str.pop_back();
	return str;
}

}

net::IP::IP() noexcept 
	: m_raw{0}
{}

net::IP::IP(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept
	: m_raw{f1, f2, f3, f4}
{}

net::IP::IP(const std::string& str) 
{
	string_to_arr4(m_raw, str);
}

net::IP::IP(const array_type& other_arr) noexcept {
	std::copy(std::begin(other_arr), std::end(other_arr), m_raw);
}

net::IP::byte_type& net::IP::raw(std::size_t index) {
	return m_raw[index];
}

const net::IP::byte_type& net::IP::raw(std::size_t index) const {
	return const_cast<IP&>(*this).raw(index);
}

net::IP::array_type& net::IP::raw_array() noexcept {
	return m_raw;
}

const net::IP::array_type& net::IP::raw_array() const noexcept {
	return const_cast<IP&>(*this).raw_array();
}

std::string net::IP::to_string() const noexcept {
	return arr4_to_string(m_raw);
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

net::MAC::byte_type& net::MAC::raw(std::size_t index) {
	return m_raw[index];
}

const net::MAC::byte_type& net::MAC::raw(std::size_t index) const {
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

net::IPMask::IPMask() noexcept
	: m_raw{0}
{}

net::IPMask::IPMask(byte_type f1, byte_type f2, byte_type f3, byte_type f4) noexcept
	: m_raw{f1, f2, f3, f4}
{}

net::IPMask::IPMask(size_type CIDR_prefix) noexcept
{
	std::fill(std::begin(m_raw), std::end(m_raw), std::numeric_limits<byte_type>::max());

	to_int() >>= CIDR_prefix;

	//++to_int();

	to_int() = (m_raw[3]<<0) | (m_raw[2]<<8) | (m_raw[1]<<16) | (m_raw[0]<<24);

	to_int() = ~to_int();
}

net::IPMask::IPMask(const std::string& str)
{
	string_to_arr4(m_raw, str);
}

net::IPMask::IPMask(const array_type& other_arr) noexcept {
	std::copy(std::begin(other_arr), std::end(other_arr), std::begin(m_raw));
}

net::IPMask::byte_type& net::IPMask::raw(std::size_t index) {
	return m_raw[index];
}

const net::IPMask::byte_type& net::IPMask::raw(std::size_t index) const {
	return m_raw[index];
}

net::IPMask::array_type& net::IPMask::raw_array() noexcept {
	return m_raw;
}

const net::IPMask::array_type& net::IPMask::raw_array() const noexcept {
	return m_raw;
}

std::string net::IPMask::to_string() const noexcept {
	return arr4_to_string(m_raw);
}

net::IPMask::size_type net::IPMask::CIDR_prefix() const noexcept
{
	auto num = this->to_int();

	size_type count = 0;
	for (;num != 0; ++count)
		num &= (num - 1);

	return count;
}

std::uint32_t& net::IPMask::to_int() noexcept {
	return *reinterpret_cast<std::uint32_t*>(m_raw);
}

const std::uint32_t& net::IPMask::to_int() const noexcept {
	return const_cast<IPMask&>(*this).to_int();
}

bool net::IPMask::operator==(const IPMask& right) const noexcept {
	return std::equal(std::begin(m_raw), std::end(m_raw), std::begin(right.m_raw));
}

bool net::IPMask::operator!=(const IPMask& right) const noexcept {
	return !(*this == right);
}

net::IP net::IPMask::operator&(net::IP ip) const noexcept
{
	std::transform(std::cbegin(ip.raw_array()), std::cend(ip.raw_array()), 
				   std::cbegin(m_raw), std::begin(ip.raw_array()), std::bit_and<byte_type>());

	return ip;
}

#if 0
static void verify_cidr(const net::CIDR::ip_type& ip, const net::CIDR::ip_mask_type& mask)
{
	static_assert(std::decay_t<decltype(ip)>::size() == std::decay_t<decltype(mask)>::size(), "");

	for (size_t i = 0; i < ip.size(); i++)
	{
		if (ip.raw(i) <= mask.raw(i)) throw std::invalid_argument("Bad IP address for the Mask");
	}
}
#endif

net::CIDR::CIDR(ip_type ip, ip_mask_type ipmask) noexcept
	: m_ip(std::move(ip)), m_mask(std::move(ipmask))
{
	//verify_cidr(m_ip, m_mask);
}

net::CIDR::CIDR(const std::string& str)
{
	const auto slash_pos = str.rfind('/');

	const auto ip_str = str.substr(0, slash_pos);
	m_ip = net::IP(ip_str);

	const auto mask_str = str.substr(slash_pos + 1);
	m_mask = net::IPMask(static_cast<net::IPMask::size_type>(std::stoi(mask_str)));

	//verify_cidr(m_ip, m_mask);
}

net::CIDR::ip_type& net::CIDR::ip() noexcept {
	return m_ip;
}

net::CIDR::ip_mask_type& net::CIDR::mask() noexcept {
	return m_mask;
}

const net::CIDR::ip_type& net::CIDR::ip() const noexcept {
	return const_cast<CIDR&>(*this).ip();
}

const net::CIDR::ip_mask_type& net::CIDR::mask() const noexcept {
	return const_cast<CIDR&>(*this).mask();
}

std::string net::CIDR::to_string() const noexcept
{
	auto ip_str = m_ip.to_string();
	ip_str += ('/' + std::to_string(m_mask.CIDR_prefix()));
	return ip_str;
}

net::CIDR::ip_type net::CIDR::subnet() const noexcept {
	return (m_mask & m_ip);
}

bool net::CIDR::operator==(const CIDR& right) const noexcept {
	return (m_ip == right.m_ip) && (m_mask == right.m_mask);
}

bool net::CIDR::operator!=(const CIDR& right) const noexcept {
	return !(*this == right);
}
