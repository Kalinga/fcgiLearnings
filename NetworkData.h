#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include <iostream>
#include <cereal/archives/json.hpp>

using namespace std;

class NetworkData {
	string ipAddress;
	string netmask;
	string gateway;

public:
	NetworkData(string ipAddress, string netmask, string gateway):
	ipAddress(ipAddress), netmask(netmask), gateway(gateway) {}

	// This method lets cereal know which data members to serialize
	template<class Archive>
	void serialize(Archive & archive)
	{
    	archive( CEREAL_NVP(ipAddress), CEREAL_NVP(netmask), CEREAL_NVP(gateway) ); // serialize things by passing them to the archive
	}
	
	void setIPAddress(string ip) {
		this->ipAddress = ip;
	}

	void setNetMask(string netmask) {
		this->netmask = netmask;
	}

	void setGateWay(string gateway) {
		this->gateway = gateway;
	}
};
#endif // NETWORK_DATA_H