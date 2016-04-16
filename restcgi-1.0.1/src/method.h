/*
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
#ifndef restcgi_method_h
#define restcgi_method_h
#include "apidefs.h"
#include "method_e.h"
#include "hdr.h"
#include "content.h"
#include "status_code_e.h"
#include <uripp/path.h>
#include <uripp/query.h>
#include <string>
#include <boost/shared_ptr.hpp>
namespace restcgi {
    class env;
    class endpoint;
    class icontent;
    class ocontent;
    /** \brief HTTP method class.
     *
     * This is returned from the \ref endpoint object
     * when the request is received and contains all the
     * information needed to determine the client request and
     * return a response.
     *
     * An application can use this class and associated classes by
     * themselves to return a response. Alternatively, the \ref resource class and functions
     * may be employed to provide a REST framework for locating a resource
     * and applying the method to it.
     *
     * For clients that cannot create \c PUT or \c DELETE HTTP requests a \c POST
     * request with query params (values ignored) of \c restPUT and \c restDELETE
     * are interpreted as such with the method object adjusted accordingly.
     *
     * Example code:
     * \code
     * #include <restcgi/endpoint.h>
     * #include <restcgi/method.h>
     * using namespace restcgi;
     * ...
     * method::pointer m = endpoint::create()->receive();
     * if (m->e() == method_e::GET) ...
     * \endcode
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616.html
     * @see endpoint, rest, resource */
    class RESTCGI_API method {
    public:
        typedef boost::shared_ptr<method> pointer; ///< shared ptr
        typedef boost::shared_ptr<const method> const_pointer; ///< const shared ptr
        typedef restcgi::env env_type; ///< env type
        typedef restcgi::endpoint endpoint_type; ///< endpoint type
        typedef boost::shared_ptr<endpoint_type> endpoint_pointer; ///< shared endpoint ptr
        typedef restcgi::icontent icontent_type; ///< iconent type
        typedef restcgi::ocontent ocontent_type; ///< oconent type
        typedef boost::shared_ptr<icontent_type> icontent_pointer; ///< shared icontent ptr
        typedef boost::shared_ptr<ocontent_type> ocontent_pointer; ///< shared ocontent ptr
        typedef restcgi::request_hdr request_hdr_type; ///< request header type
        typedef restcgi::response_hdr response_hdr_type; ///< response header type
        typedef uripp::path uri_path_type; ///< URI path type
        typedef uripp::query uri_query_type; ///< URI query type
        virtual ~method();
        const method_e& e() const {return e_;} ///< Get method enumeration.
        /** \brief Get URI path.
         *
         * This is the path starting after the CGI script name,
         * i.e. it is the env::path_info(). */
        const uri_path_type& uri_path() const {return uri_path_;}
        const uri_query_type& uri_query() const {return uri_query_;} ///< Get URI query.
        const request_hdr_type& request_hdr() const {return request_hdr_;} ///< Get request header.
        icontent_pointer icontent() const {return icontent_;} ///< Get input content stream.
        ocontent_pointer ocontent() const {return ocontent_;} ///< Get output content stream (must respond first).
        /** \brief Send the response header, no content will be sent.
         *
         * @exception std::domain_error if already responded */
        void respond(const status_code_e& sc = status_code_e::OK, const response_hdr_type& rh = response_hdr_type());
        /** \brief  Send the response and content header and create the
         * stream for sending the content itself.
         *
         * @exception std::domain_error if already responded */
        ocontent_pointer respond(const content_hdr& ch, const status_code_e& sc = status_code_e::OK, const response_hdr_type& rh = response_hdr_type());
        bool responded() const {return responded_;} ///< Test if responded.
        const env_type& env() const; ///< Get environment.
        endpoint_pointer endpoint() const {return endpoint_;} ///< Get endpoint.
        static const char QP_REST_PUT[8]; ///< query param for PUT method ("restPUT")
        static const char QP_REST_DELETE[11]; ///< query param for DELETE method ("restDELETE")
    private:
        method(const method&); // inhibit
        method& operator =(const method&); // inhibit
        method(const method_e& e, const endpoint_pointer& ep);
        friend class endpoint;
        /// Factory function for use by endpoint.
        static pointer create(const endpoint_pointer& ep);
        void respond(const status_code_e& sc, const response_hdr_type& rh, const content_hdr& ch, bool has_content);
        static void respond(endpoint_pointer ep, const status_code_e& sc, const response_hdr_type& rh = response_hdr_type(), const content_hdr& ch = content_hdr());
        method_e e_;
        /// Keep endpoint around as long as we are.
        endpoint_pointer endpoint_;
        uri_path_type uri_path_;
        uri_query_type uri_query_;
        bool responded_;
        request_hdr_type request_hdr_;
        icontent_pointer icontent_;
        ocontent_pointer ocontent_;
    };
}
#endif
