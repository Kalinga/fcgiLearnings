#ifndef MYROOT_H
#define MYROOT_H

#include <restcgi/resource.h>

class Myroot : public restcgi::resource {
public:
	Myroot(int methods_allowed_mask):
	restcgi::resource (methods_allowed_mask) {

	}
	
};
#endif //MYROOT