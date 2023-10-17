#include <packet.hpp>

#include <gtest/gtest.h>

namespace {

struct packet : ::testing::Test {
	ns::packet A{10};
};

TEST_F(packet, data) {
	EXPECT_NE(A.data(), nullptr);
	EXPECT_NE(A.data_u8(), nullptr);
}
TEST_F(packet, subscript) {
	A[0] = 1;
	A[1] = 2;
	A[2] = 100;
	EXPECT_EQ(A[0], 1);
	EXPECT_EQ(A[1], 2);
	EXPECT_EQ(A[2], 100);
}
TEST_F(packet, size) {
	EXPECT_EQ(A.size(), 10);
}
TEST_F(packet, copy) {
	A[0] = 255;
	auto B = A.copy();
	B[0] = 245;
	EXPECT_EQ(A[0], 255);
	EXPECT_EQ(A.size(), 10);
	EXPECT_EQ(B[0], 245);
	EXPECT_EQ(B.size(), 10);
}

}