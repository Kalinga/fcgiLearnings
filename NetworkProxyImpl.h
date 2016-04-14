
#ifndef NETWORKPROXY_IMPL_H
#define NETWORKPROXY_IMPL_H

#include "NetworkProxy.h"

class NetworkManager_proxyImpl : public org::freedesktop::NetworkManager_proxy,
				public DBus::IntrospectableProxy,
				public DBus::ObjectProxy
{
public:
	NetworkManager_proxyImpl(DBus::Connection &connection, const char *path, const char *name):
	DBus::ObjectProxy(connection, path, name)
	{
	}

	void DeviceRemoved(const ::DBus::Path& argin0) {}
    void DeviceAdded(const ::DBus::Path& argin0) {}
    void PropertiesChanged(const std::map< std::string, ::DBus::Variant >& argin0) {}
    void StateChanged(const uint32_t& argin0) {}
    void CheckPermissions() {}

};
#endif //NETWORKPROXY_IMPL_H