// Copyright zooml.com 2008 All rights reserved.
#ifndef restcgi_endpoint_h
#define restcgi_endpoint_h
#include "apidefs.h"
#include "env.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
namespace restcgi {
    class method;
    /** \brief Represents the HTTP service endpoint.
     *
     * When a request is received
     * from a client a method object is created to represent the
     * request method and provide a mechanism for responding.
     *
     * Note that the handler class hides some of the
     * details of responding.
     * The returned method can be passed to an application-defined
     * root handler object to make it easier to find the resource
     * identified by the URI and execute the method on it.
     * @see rest, method, resource, apply() */
    class RESTCGI_API endpoint {
    public:
        typedef boost::shared_ptr<endpoint> pointer; ///< shared ptr
        typedef boost::shared_ptr<const endpoint> const_pointer; ///< const shared ptr
        typedef restcgi::env env_type; ///< env type
        typedef boost::shared_ptr<method> method_pointer; ///< method shared ptr
        ~endpoint();
        /// Receive method. Conceptually, this receives the method request
        /// header fields. Received content, if any, can be streamed after
        /// this call. Response header and content can be sent using the
        /// returned method object.
        /// @see method_e
        /// @exception std::domain_error if method type unknown (response automatically sent)
        method_pointer receive();
        const env_type& env() const {return env_;} ///< Get environment.
        void env_override(const std::string& name, const std::string& value); ///< Override env.
        static pointer create(); ///< Create endpoint from std::cin and std::cout.
        static pointer create(std::istream& is, std::ostream& os); ///< Create endpoint.
        static pointer create(const env_type& e, std::istream& is, std::ostream& os); ///< For testing.
    private:
        endpoint(const endpoint&); // inhibit
        endpoint& operator =(const endpoint&); // inhibit
        endpoint(const env_type& e, std::istream& is, std::ostream& os);
        boost::weak_ptr<endpoint> this_;
        env_type env_;
        friend class method;
        friend class icontent;
        friend class ocontent;
        std::istream& is_;
        std::ostream& os_;
    };
}
#endif
