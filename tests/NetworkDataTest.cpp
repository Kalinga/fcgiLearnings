#include <gtest/gtest.h>

#include "../NetworkData.h"

using namespace std;

TEST(NetworkDataTest, serialization) { 

	string ip("172.17.0.1");
	string mask("255.0.0.0");
	string gateway("192.168.1.1");

	NetworkData * data = new NetworkData(ip, mask, gateway);

	ASSERT_NE(string::npos, data->serialize().find(ip));
	ASSERT_NE(string::npos, data->serialize().find(mask));
	ASSERT_NE(string::npos, data->serialize().find(gateway));
     
}