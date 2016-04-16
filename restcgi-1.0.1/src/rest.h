#ifndef restcgi_rest_h
#define restcgi_rest_h
#include "apidefs.h"
#include "ctmpl.h"
#include <map>
#include <boost/shared_ptr.hpp>
/** \mainpage restcgi - REST CGI C++ Library
 *
 * See <a href="classrestcgi_1_1rest.html">rest</a> class for introduction and example.
 *
 * \section feat_sec Features
 * <ol>
 * <li>Hierarchical resources.</li>
 * <li>Resource versioning with etags and timestamps.</li>
 * <li>Cookies.</li>
 * <li>RFC-compliant HTTP syntax including full parsing of headers.</li>
 * <li>Streaming message bodies.</li>
 * <li>CGI/Fast CGI support.</li>
 * </ol>
 *
 * \section req_sec Requirements
 * <ol>
 * <li><a href="http://en.wikipedia.org/wiki/C%2B%2B_standard_library">standard library</a></li>
 * <li><a href="http://www.boost.org/">boost library</a></li>
 * <li><a href="https://sourceforge.net/projects/uripp/">uripp - URI C++ library</a></li>
 * <li>Runtime: <a href="http://fastcgi.coremail.cn/">mod_fcgi</a> or
 * 	   <a href="http://www.fastcgi.com/drupal/">mod_fastcgi</a></li>
 * </ol>
 *
 * \section lic_sec License (MIT)
Copyright (c) 2009 zooml.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

/** \brief  REST CGI library.
 *
 * The library contains
 * classes specifically for REST processing, which are built
 * on classes that provide basic CGI functionality. The basic
 * CGI functionality is documented in the method class
 * where there is example code for using CGI without REST.
 * The rest class contains the documentation for the REST classes
 * that are built on and hide some of the basic CGI classes.
 * @see method, rest */
namespace restcgi {
    class method;
    class resource;
    /** \brief REST processing for application resources.
     *
     * This uses the underlying CGI functionality of method
     * and its associated classes to call classes derived from
     * the \ref resource base class when an HTTP request is received.
     *
     * The basic model is that the application uses the resource
     * class as a base classe to code its hierarchically
     * contained resources, or their proxies.
     * The process() method of this class then handles all framework
     * processing except for the method responses for the resources.
     * These are the steps, implemented as overridable methods:
     * <ol><li>special_case(): Check for and send responses for special cases.</li>
     * <li>If not special cases:
     *     <ol><li>locate(): Locate resource specified by the URI path in the method.</li>
     *     <li>apply(): Call the appropriate method on the resource.</li></ol>
     * </li>
     * <li>on_exception(): Exception handler to send the error response
     *     using the provided content templates (ctmpl).</li>
     * </ol>
     *
     * Example code using <a href="http://www.fastcgi.com/drupal/">FastCGI</a>:
     * The application-specific class \c myrootrsc is
     * derived from the \ref resource class to represent its root resource.
     *
     * \code
     * #include <fcgio.h>
     * #include <restcgi/endpoint.h>
     * #include <restcgi/rest.h>
     * #include "myrootrsc.h"
     * int main(int argc, char* argv[]) {
     *     // FastCGI initialization.
     *     FCGX_Init();
     *     FCGX_Request request;
     *     FCGX_InitRequest(&request, 0, 0);
     *     while (FCGX_Accept_r(&request) >= 0) {
     *         // FastCGI request setup.
     *         fcgi_streambuf fisbuf(request.in);
     *         std::istream is(&fisbuf);
     *         fcgi_streambuf fosbuf(request.out);
     *         std::ostream os(&fosbuf);
     *         set_env(request.envp);
     *         // restcgi processing. Note rest::process() catches exceptions and 
     *         // translates them to HTTP error status codes (derive from rest to customize).
     *         restcgi::endpoint::method_pointer m = restcgi::endpoint::create(is, os)->receive();
     *         restcgi::resource::pointer root(new myrootrsc());
     *         restcgi::rest().process(m, root);
     *     }
     *     return 0;
     * }
     * void set_env(char** penv) {
     *     char buf[4000];
     *     for (; *penv; ++penv) {
     *         ::strncpy(buf, *penv, sizeof(buf));
     *         buf[sizeof(buf) - 1] = 0; // ensure terminated
     *         char* p = ::strchr(buf, '=');
     *         const char* v = "";
     *         if (p) {*p = 0; v = p + 1;}
     *         ::setenv(buf, v, 1); // makes copy of buf, v
     *     }
     * }
     * \endcode
     * @see resource, method */
    class RESTCGI_API rest {
    public:
        typedef restcgi::method method_type; ///< method type
        typedef boost::shared_ptr<method_type> method_pointer; ///< method shared ptr
        typedef restcgi::resource resource_type; ///< resource type
        typedef boost::shared_ptr<resource_type> resource_pointer; ///< resource shared ptr
        typedef std::map<std::string, resource_pointer> vhosts_resources_type; ///< virtual hosts resources type
        rest(); ///< Construct.
        virtual ~rest();
        /** \brief Perform processing of the given method starting at the
         * given root resource.
         *
         * Uses the optional templates on exceptions.
         * See class description. This does not throw any exception. */
        void process(method_pointer m, resource_pointer root, const sc_ctmpls& sccts = sc_ctmpls());
        /** \brief Perform processing of the given method for the root resouce
         * that matches the HTTP_HOST header.
         *
         * This uses the domain name/IP addr plus the ":<port>" if any. (See also other process()). */
        void process(method_pointer m, const vhosts_resources_type& vhrs);
        method_pointer method() const {return method_;} ///< Get method (valid after process()).
        resource_pointer root() const {return root_;} ///< Get root resource (valid after process()).
        resource_pointer resource() const {return resource_;} ///< Get resource (valid after locate()).
        static bool special_case(method_pointer m); ///< Default special case.
        static resource_pointer locate(method_pointer m, resource_pointer& r); ///< Default locate.
        static void apply(method_pointer m, resource_pointer r); ///< Default apply.
        static void on_exception(method_pointer m, const sc_ctmpls& sccts); ///< Default exception handler.
    protected:
        /// Detect and handle special case methods and return whether
        /// special case or not. This handles OPTIONS "*" by sending
        /// an empty response ("ping"-like behavior).
        virtual bool special_case();
        /// Locate the resource for the method's URI path. This will start
        /// with the root and call resource::locate() and the resource it returns,
        /// transitively, until either no resource is returned (the previous
        /// is used in this case) or uri path is empty. The resource arg
        /// should be updated as the algorithm progresses so it is left
        /// at the last known good point if an exception is thrown.
        /// Sets resource pointer member as it progresses.
        /// @see resource::locate
        /// @exception not_found
        virtual resource_pointer locate(resource_pointer& r);
        /// Apply the method to the resource. This examines the method
        /// type and calls the corresponding resource method to create
        /// a response, e.g. calls resource::get() if the method is a GET.
        /// @see resource
        /// @exception exception many different kinds if error in processing
        virtual void apply();
        /// Handle response for exception during processing. This first tests
        /// that a method has been received and that there is no response yet.
        /// It (internally) rethrows the last exception and then produces
        /// an appropriate response based on the exception and the provided
        /// content template for the exception based on its status code.
        /// @see exception
        virtual void on_exception(const sc_ctmpls& sccts);
    private:
        method_pointer method_;
        resource_pointer root_;
        resource_pointer resource_;
    };
}
#endif
