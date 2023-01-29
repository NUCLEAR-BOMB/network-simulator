#include "Tables.hpp"

#include <stdexcept>

const net::MAC* net::ARPTable::find(const net::IP& ip) const noexcept
{
    auto res = m_container.find(ip);

    if (res == m_container.end()) return nullptr;

    return &res->second;
}

bool net::ARPTable::add_back(net::IP ip, net::MAC mac) noexcept
{
    for (auto it = m_container.begin(); it != m_container.end(); ++it)
    {
        if (it->first == ip) return false;
        
        if (it->second == mac)
        {
            m_container.erase(it);

            m_container.emplace(std::move(ip), std::move(mac));

            return true;
        }
    }

    m_container.emplace(std::move(ip), std::move(mac));

    return true;
}

net::RoutingTable::container_value_type& net::RoutingTable::find(const net::IP& ip)
{
    for (auto& elem : m_container)
    {
        if ((elem.mask & ip) == elem.ip) return elem;
    }

    throw std::runtime_error("No mathes in the routing table");
}



void net::RoutingTable::add_back(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept
{
    auto res = std::find_if(m_container.begin(), m_container.end(), [&](const auto& val) {
        return (val.ip == ip);
    });
    if (res != m_container.end()) return;

    m_container.push_back(container_value_type{ std::move(ip), std::move(mask), interface });
}

void net::RoutingTable::add_front(net::IP ip, net::IPMask mask, net::Interface& interface) noexcept
{
    auto res = std::find_if(m_container.begin(), m_container.end(), [&](const auto& val) {
        return (val.ip == ip);
       });
    if (res != m_container.end()) return;

    m_container.push_front(container_value_type{ std::move(ip), std::move(mask), interface });
}
