
# RELEASING: update this version number according to interface changes
# see http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info
PACKAGE_LIBRARY_VERSION=0:0:0

lib_LTLIBRARIES = librestcgi.la
pkginclude_HEADERS = apidefs.h content.h cookie.h ctmpl.h date_time.h endpoint.h env.h exception.h hdr.h httpsyn.h method_e.h method.h resource.h rest.h status_code_e.h utils.h version.h
librestcgi_la_SOURCES = content.cpp cookie.cpp ctmpl.cpp date_time.cpp endpoint.cpp env.cpp exception.cpp hdr.cpp httpsyn.cpp method.cpp method_e.cpp resource.cpp rest.cpp status_code_e.cpp utils.cpp version.cpp
librestcgi_la_LDFLAGS = -version-info $(PACKAGE_LIBRARY_VERSION)
