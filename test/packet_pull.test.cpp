#include <packet_pull.hpp>

#include <gtest/gtest.h>

namespace {

struct packet_pull : ::testing::Test {
	ns::packet_pull pull;

	void SetUp() override {
		auto pac1 = ns::packet(5);
		pac1[0] = 1;
		pac1[1] = 2;
		pull.push(pac1);

		auto pac2 = ns::packet(1);
		pac2[0] = 100;
		pull.push(pac2);
	}
};

TEST_F(packet_pull, size) {
	EXPECT_EQ(pull.size(), 2);
}
TEST_F(packet_pull, subscript) {
	EXPECT_EQ(pull[0][0], 1);
	EXPECT_EQ(pull[0][1], 2);

	EXPECT_EQ(pull[1][0], 100);
}

TEST_F(packet_pull, push) {
	EXPECT_EQ(pull.size(), 2);
	auto pac3 = ns::packet(100);
	pac3[5] = 0xFF;
	pull.push(pac3);
	EXPECT_EQ(pull.size(), 3);
}

}