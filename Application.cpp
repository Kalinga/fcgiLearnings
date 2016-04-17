#include <iostream>
#include <unistd.h>

#include "fcgio.h"

#include <restcgi/endpoint.h>
#include <restcgi/rest.h>

#include "NetworkProxyImpl.h"
#include "NetworkData.h"
#include "Myroot.h"

using namespace std;
using namespace org::freedesktop;

#define FCGI_ONLY 0

DBus::BusDispatcher dispatcher;

extern "C" {
     void set_env(char** penv) {
         char buf[4000];
         for (; *penv; ++penv) {
             ::strncpy(buf, *penv, sizeof(buf));
             buf[sizeof(buf) - 1] = 0; // ensure terminated
             char* p = ::strchr(buf, '=');
             const char* v = "";
             if (p) {*p = 0; v = p + 1;}
             ::setenv(buf, v, 1); // makes copy of buf, v
         }
    }
}

int main(void) {
#if FCGI_ONLY
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();
#endif

    FCGX_Init();
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
#if FCGI_ONLY
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);
#endif

        // TODO: Code required refactoring and should be moved from here
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

#if FCGI_ONLY
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
#endif
        //REST
        fcgi_streambuf fisbuf(request.in);
        std::istream is(&fisbuf);
        fcgi_streambuf fosbuf(request.out);
        std::ostream os(&fosbuf);
        set_env(request.envp);

         // restcgi processing. 
        restcgi::endpoint::method_pointer m = restcgi::endpoint::create(is, os)->receive();
        restcgi::resource::pointer root(new Myroot( restcgi::method_e::GET));
        restcgi::rest().process(m, root);

        // Note: the fcgi_streambuf destructor will auto flush
    }

#if FCGI_ONLY
    // restore stdio streambufs
    /*cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);*/
#endif
    return 0;
}