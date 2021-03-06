
/*
 *	This file was automatically generated by dbusxx-xml2cpp; DO NOT EDIT!
 */

#ifndef __dbusxx__NetworkProxy_h__PROXY_MARSHAL_H
#define __dbusxx__NetworkProxy_h__PROXY_MARSHAL_H

#include <dbus-c++/dbus.h>
#include <cassert>

namespace org {
namespace freedesktop {

class NetworkManager_proxy
: public ::DBus::InterfaceProxy
{
public:

    NetworkManager_proxy()
    : ::DBus::InterfaceProxy("org.freedesktop.NetworkManager")
    {
        connect_signal(NetworkManager_proxy, DeviceRemoved, _DeviceRemoved_stub);
        connect_signal(NetworkManager_proxy, DeviceAdded, _DeviceAdded_stub);
        connect_signal(NetworkManager_proxy, PropertiesChanged, _PropertiesChanged_stub);
        connect_signal(NetworkManager_proxy, StateChanged, _StateChanged_stub);
        connect_signal(NetworkManager_proxy, CheckPermissions, _CheckPermissions_stub);
    }

public:

    /* properties exported by this interface */
        const uint32_t Connectivity() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "Connectivity";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const uint32_t State() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "State";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const std::string Version() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "Version";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const ::DBus::Path ActivatingConnection() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "ActivatingConnection";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const ::DBus::Path PrimaryConnection() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "PrimaryConnection";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const std::vector< ::DBus::Path > ActiveConnections() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "ActiveConnections";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const bool WimaxHardwareEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WimaxHardwareEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const bool WimaxEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WimaxEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        void WimaxEnabled( const bool & input) {
            ::DBus::CallMessage call ;
             call.member("Set");  call.interface( "org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            ::DBus::Variant value;
            ::DBus::MessageIter vi = value.writer ();
            vi << input;
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WimaxEnabled";
            wi << interface_name;
            wi << property_name;
            wi << value;
            ::DBus::Message ret = this->invoke_method (call);
        };
        const bool WwanHardwareEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WwanHardwareEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const bool WwanEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WwanEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        void WwanEnabled( const bool & input) {
            ::DBus::CallMessage call ;
             call.member("Set");  call.interface( "org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            ::DBus::Variant value;
            ::DBus::MessageIter vi = value.writer ();
            vi << input;
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WwanEnabled";
            wi << interface_name;
            wi << property_name;
            wi << value;
            ::DBus::Message ret = this->invoke_method (call);
        };
        const bool WirelessHardwareEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WirelessHardwareEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        const bool WirelessEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WirelessEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
        void WirelessEnabled( const bool & input) {
            ::DBus::CallMessage call ;
             call.member("Set");  call.interface( "org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            ::DBus::Variant value;
            ::DBus::MessageIter vi = value.writer ();
            vi << input;
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "WirelessEnabled";
            wi << interface_name;
            wi << property_name;
            wi << value;
            ::DBus::Message ret = this->invoke_method (call);
        };
        const bool NetworkingEnabled() {
            ::DBus::CallMessage call ;
             call.member("Get"); call.interface("org.freedesktop.DBus.Properties");
            ::DBus::MessageIter wi = call.writer(); 
            const std::string interface_name = "org.freedesktop.NetworkManager";
            const std::string property_name  = "NetworkingEnabled";
            wi << interface_name;
            wi << property_name;
            ::DBus::Message ret = this->invoke_method (call);
            ::DBus::MessageIter ri = ret.reader ();
            ::DBus::Variant argout; 
            ri >> argout;
            return argout;
        };
public:

    /* methods exported by this interface,
     * this functions will invoke the corresponding methods on the remote objects
     */
    uint32_t state()
    {
        ::DBus::CallMessage call;
        call.member("state");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        uint32_t argout;
        ri >> argout;
        return argout;
    }

    uint32_t CheckConnectivity()
    {
        ::DBus::CallMessage call;
        call.member("CheckConnectivity");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        uint32_t argout;
        ri >> argout;
        return argout;
    }

    void GetLogging(std::string& level, std::string& domains)
    {
        ::DBus::CallMessage call;
        call.member("GetLogging");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        ri >> level;
        ri >> domains;
    }

    void SetLogging(const std::string& level, const std::string& domains)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << level;
        wi << domains;
        call.member("SetLogging");
        ::DBus::Message ret = invoke_method (call);
    }

    std::map< std::string, std::string > GetPermissions()
    {
        ::DBus::CallMessage call;
        call.member("GetPermissions");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        std::map< std::string, std::string > argout;
        ri >> argout;
        return argout;
    }

    void Enable(const bool& enable)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << enable;
        call.member("Enable");
        ::DBus::Message ret = invoke_method (call);
    }

    void Sleep(const bool& sleep)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << sleep;
        call.member("Sleep");
        ::DBus::Message ret = invoke_method (call);
    }

    void DeactivateConnection(const ::DBus::Path& active_connection)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << active_connection;
        call.member("DeactivateConnection");
        ::DBus::Message ret = invoke_method (call);
    }

    void AddAndActivateConnection(const std::map< std::string, std::map< std::string, ::DBus::Variant > >& connection, const ::DBus::Path& device, const ::DBus::Path& specific_object, ::DBus::Path& path, ::DBus::Path& active_connection)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << connection;
        wi << device;
        wi << specific_object;
        call.member("AddAndActivateConnection");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        ri >> path;
        ri >> active_connection;
    }

    ::DBus::Path ActivateConnection(const ::DBus::Path& connection, const ::DBus::Path& device, const ::DBus::Path& specific_object)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << connection;
        wi << device;
        wi << specific_object;
        call.member("ActivateConnection");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        ::DBus::Path argout;
        ri >> argout;
        return argout;
    }

    ::DBus::Path GetDeviceByIpIface(const std::string& iface)
    {
        ::DBus::CallMessage call;
        ::DBus::MessageIter wi = call.writer();

        wi << iface;
        call.member("GetDeviceByIpIface");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        ::DBus::Path argout;
        ri >> argout;
        return argout;
    }

    std::vector< ::DBus::Path > GetDevices()
    {
        ::DBus::CallMessage call;
        call.member("GetDevices");
        ::DBus::Message ret = invoke_method (call);
        ::DBus::MessageIter ri = ret.reader();

        std::vector< ::DBus::Path > argout;
        ri >> argout;
        return argout;
    }


public:

    /* signal handlers for this interface
     */
    virtual void DeviceRemoved(const ::DBus::Path& argin0) = 0;
    virtual void DeviceAdded(const ::DBus::Path& argin0) = 0;
    virtual void PropertiesChanged(const std::map< std::string, ::DBus::Variant >& argin0) = 0;
    virtual void StateChanged(const uint32_t& argin0) = 0;
    virtual void CheckPermissions() = 0;

private:

    /* unmarshalers (to unpack the DBus message before calling the actual signal handler)
     */
    void _DeviceRemoved_stub(const ::DBus::SignalMessage &sig)
    {
        ::DBus::MessageIter ri = sig.reader();

        ::DBus::Path arg0;
        ri >> arg0;
        DeviceRemoved(arg0);
    }
    void _DeviceAdded_stub(const ::DBus::SignalMessage &sig)
    {
        ::DBus::MessageIter ri = sig.reader();

        ::DBus::Path arg0;
        ri >> arg0;
        DeviceAdded(arg0);
    }
    void _PropertiesChanged_stub(const ::DBus::SignalMessage &sig)
    {
        ::DBus::MessageIter ri = sig.reader();

        std::map< std::string, ::DBus::Variant > arg0;
        ri >> arg0;
        PropertiesChanged(arg0);
    }
    void _StateChanged_stub(const ::DBus::SignalMessage &sig)
    {
        ::DBus::MessageIter ri = sig.reader();

        uint32_t arg0;
        ri >> arg0;
        StateChanged(arg0);
    }
    void _CheckPermissions_stub(const ::DBus::SignalMessage &sig)
    {
        CheckPermissions();
    }
};

} } 
#endif //__dbusxx__NetworkProxy_h__PROXY_MARSHAL_H
