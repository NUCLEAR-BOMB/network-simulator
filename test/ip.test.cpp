
#include <ip.hpp>

#include <gtest/gtest.h>

namespace {

TEST(ip, from_str) {
	EXPECT_EQ(ns::ip("1.2.3.4"), ns::ip(1, 2, 3, 4));
	EXPECT_EQ(ns::ip("255.255.255.255"), ns::ip(255, 255, 255, 255));
	EXPECT_EQ(ns::ip("0.0.0.0"), ns::ip(0, 0, 0, 0));
}
TEST(ip, to_string) {
	EXPECT_EQ(ns::to_string(ns::ip("1.2.3.4")), "1.2.3.4");
	EXPECT_EQ(ns::to_string(ns::ip("255.255.255.255")), "255.255.255.255");
}

}
