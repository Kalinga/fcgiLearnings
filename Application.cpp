#include <iostream>

#include "fcgio.h"

#include "NetworkProxyImpl.h"
#include "NetworkData.h"

using namespace std;
using namespace org::freedesktop;

DBus::BusDispatcher dispatcher;

int main(void) {
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        DBus::default_dispatcher = &dispatcher;
        DBus::Connection bus = DBus::Connection::SystemBus();

        NetworkManager_proxyImpl proxy(bus, "/org/freedesktop/NetworkManager",
                                            "org.freedesktop.NetworkManager");
        bool isNetwokEnabled = false;
        isNetwokEnabled = proxy.NetworkingEnabled();

        bool isWirelessEnabled = false;
        isWirelessEnabled = proxy.WirelessEnabled();
        
        std::vector< ::DBus::Path > connections = proxy.ActiveConnections();
        std::stringstream ss;
        for(std::vector< ::DBus::Path >::const_iterator itr = connections.begin();  itr != connections.end(); ++itr) {
            std::string path = (*itr);
            ss<<path;
        }

        std::vector< ::DBus::Path > devices = proxy.GetDevices();
        std::stringstream deviceList;
        for(std::vector< ::DBus::Path >::const_iterator itr = devices.begin();  itr != devices.end(); ++itr) {
            std::string path = (*itr);
            deviceList<<path;
        }
       
        // TODO: Create the Proxy for "org.freedesktop.NetworkManager.Device" using object path from above
        // TODO: Fetch org.freedesktop.NetworkManager.Device.Ip4Config which gives object path
        // for org.freedesktop.NetworkManager.Ip4Config and create a proxy for 'org.freedesktop.NetworkManager.Ip4Config' interface
        // org.freedesktop.NetworkManager.Ip4Config.AddressData and Gateway gives required data.

        // TODO: Remove the hardcoded values once the above steps are done
        string ip("172.17.0.1");
        string mask("255.0.0.0");
        string gateway("192.168.1.1");
        NetworkData * data = new NetworkData(ip, mask, gateway);

        std::stringstream stream; // any stream can be used

        cereal::JSONOutputArchive oarchive(stream); // Create an output archive

        oarchive(*data); // Write the data to the archive
    
    if (isNetwokEnabled)    {
        cout << "Content-type: text/html\r\n"
             << "\r\n"
             << "<html>\n"
             << "  <head>\n"
             << "    <title>Hello, World!</title>\n"
             << "  </head>\n"
             << "  <body>\n"
             << "    <h1>Hello, Network is enabled in client system!</h1>\n"
             << "    <h1>"
             << "        isWirelessEnabled: " <<isWirelessEnabled
             << "    </h1>\n"
             << "    <h1>"
             << "        ActiveConnections: " << ss.str()
             << "    </h1>\n"
             << "    <h1>"
             << "        DeviceList: " << deviceList.str()
             << "    </h1>\n"
             << "    <h1>"
             << "        Configuration: " << stream.str()
             << "    </h1>\n"

             << "  </body>\n"
             << "</html>\n";
         } else {
         cout << "Content-type: text/html\r\n"
             << "\r\n"
             << "<html>\n"
             << "  <head>\n"
             << "    <title>Hello, World!</title>\n"
             << "  </head>\n"
             << "  <body>\n"
             << "    <h1>Hello, Network is NOT enabled in client system!</h1>\n"
             << "  </body>\n"
             << "</html>\n";
         }

        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}