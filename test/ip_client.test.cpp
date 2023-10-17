
#include <ip_client.hpp>

#include <gtest/gtest.h>

namespace {

struct ip_client : ::testing::Test {
	std::shared_ptr<ns::ip_client> clientA;
	std::shared_ptr<ns::ip_client> clientB;
	std::shared_ptr<ns::ip_client> clientC;

	void SetUp() override {
		clientA = std::make_shared<ns::ip_client>(ns::ip("192.168.1.2"));
		clientB = std::make_shared<ns::ip_client>(ns::ip("192.168.1.3"));
		clientC = std::make_shared<ns::ip_client>(ns::ip("192.168.1.4"));

		clientA->add_connection(clientB);
		clientA->add_connection(clientC);
		clientB->add_connection(clientC);
	}
};

TEST_F(ip_client, send_message) {
	clientA->send_message_to(ns::ip("192.168.1.3"), "hello!");
	EXPECT_EQ(clientB->received().last(), ns::packet({'h', 'e', 'l', 'l', 'o', '!'}));

	clientB->send_message_to(ns::ip("192.168.1.2"), "!");
	EXPECT_EQ(clientA->received().last(), ns::packet({{'!'}}));
}


}
