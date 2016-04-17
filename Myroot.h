#ifndef MYROOT_H
#define MYROOT_H

#include <restcgi/resource.h>
#include <restcgi/version.h>

#include "NetworkData.h"

using namespace restcgi;

class Myroot : public restcgi::resource {
public:
	Myroot(int methods_allowed_mask):
	restcgi::resource (methods_allowed_mask) {

	}

	// Respond with a representation of this resource in the output content.
	// usually not necessary to override this.
	/*void get_method() {
	}*/

    version read(bool veronly) {
		cout<<"read\n";
    	return version();
    }

    void on_responding(status_code_e& sc, response_hdr& rh, content_hdr& ch) {
    	cout<<"on_responding\n";
    
    	string ip("172.17.0.1");
        string mask("255.0.0.0");
        string gateway("192.168.1.1");
        NetworkData * data = new NetworkData(ip, mask, gateway);

        {
            cereal::JSONOutputArchive oarchive(stream); // Create an output archive
            oarchive(*data); // Write the data to the archive
        }

        ch.content_type("application/json");
        ch.content_length(stream.str().length() + 1);
    }

    void write(ocontent::pointer oc) {
        cout<<"write\n";

        *oc << stream.str();
    }

	//Needs to check the usage of this interface
    /*pointer locate(uri_path_type& path) {
    	cout<<"locate\n";
	    uri_path_ = path;
	    path.clear();
	    return pointer();
    }*/

	std::stringstream stream; // any stream can be used
    uri_path_type uri_path_;    
	
};
#endif //MYROOT