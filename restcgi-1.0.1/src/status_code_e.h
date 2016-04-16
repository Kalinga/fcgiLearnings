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
#ifndef restcgi_status_code_e_h
#define restcgi_status_code_e_h
#include "apidefs.h"
#include <string>
#include <iostream>
namespace restcgi {
    /** \brief HTTP status code enumeration.
     *
     * Note that the exception classes
     * and exception_responder can be used to send the status code
     * automatically when exceptions are thrown.
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html */
    class RESTCGI_API status_code_e {
    public:
        /// Enumeration.
        enum e {
            null, ///< none
            CONTINUE, ///< Continue
            SWITCHING_PROTOCOLS, ///< Switching protocols
            /// OK (successful): Set automatically upon successful
            /// completion of all responses except POST.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.2.1
            OK,
            /// Created (successful): Application should set in the POST
            /// response upon successful completion.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.2.2
            CREATED,
            ACCEPTED, ///< Accepted
            NON_AUTHORITATIVE_INFORMATION, ///< Non-authoritative information
            /// No content (successful): The server has fulfilled the request but does
            /// not need to return an entity-body, and might want to return updated
            /// metainformation. The response MAY include new or updated
            /// metainformation in the form of entity-headers, which if present
            /// SHOULD be associated with the requested variant.
            /// If the client is a user agent, it SHOULD NOT change its document
            /// view from that which caused the request to be sent. This response
            /// is primarily intended to allow input for actions to take place without
            /// causing a change to the user agent's active document view, although any
            /// new or updated metainformation SHOULD be applied to the document
            /// currently in the user agent's active view. Must not include (entity) content.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.2.5
            NO_CONTENT,
            /// Reset content (successful): The server has fulfilled the request
            /// and the user agent SHOULD reset the document view which caused the
            /// request to be sent. This response is primarily intended to allow
            /// input for actions to take place via user input, followed by a clearing
            /// of the form in which the input is given so that the user can easily
            /// initiate another input action. Must not include (entity) content.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.2.6
            RESET_CONTENT,
            PARTIAL_CONTENT, ///< Partial content
            MULTIPLE_CHOICES, ///< Multiple choices
            MOVED_PERMANENTLY, ///< Moved permanently
            FOUND, ///< Fount
            /// See Other (redirection): The response to the request can
            /// be found under a different URI and SHOULD be retrieved using
            /// a GET method on that resource. The different URI SHOULD be
            /// given by the Location field in the response. Unless the
            /// request method was HEAD, the entity of the response SHOULD
            /// contain a short hypertext note with a hyperlink to the new URI(s).
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.3.4
            SEE_OTHER,
            /// Not Modified (redirection): Set automatically when ????????????
            /// resource does not meet the modified constraint.
            /// ETag and/or Content-Location must also be sent.
            /// Must not include (entity) content.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.3.5
            NOT_MODIFIED,
            USE_PROXY, ///< Use proxy
            TEMPORARY_REDIRECT, ///< Temporary redirect
            /// Bad request (client): endpoint sets automatically when the
            /// request header is malformed, or set when
            /// the path is unrecognized (not just a missing resource
            /// due to a bad id). Application can also set if the
            /// path or query is unrecognized, include error content.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.1
            BAD_REQUEST,
            /// Unauthorized (client): Application should set/throw if the request
            /// requires user authentication and none is provided.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.2
            UNAUTHORIZED,
            PAYMENT_REQUIRED, ///< Payment required
            FORBIDDEN, ///< Forbidden
            /// Not Found (client): Set automatically when
            /// a resource is not found. Note that BAD_REQUEST should
            /// be used if there is an error in the path or query
            /// (not just an unresolvable resource id).
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.5
            NOT_FOUND,
            /// Method Not Allowed (client): resource sets automatically
            /// when method not supported. Must include an allow header
            /// containing list of valid methods for the resource.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.6
            METHOD_NOT_ALLOWED,
            NOT_ACCEPTABLE, ///< Not acceptable
            PROXY_AUTHENTICATION_REQUIRED, ///< Proxy authentication required
            REQUEST_TIME_OUT, ///< Request time out
            /// Conflict (client): Application should set/throw if the request
            /// could not be completed due to a conflict with the current
            /// state of the resource. For example, if there is a dependency
            /// which is unmet, say, before deleting an resource. Application
            /// should include error content identifying the conflict
            /// and remedies.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.10
            CONFLICT,
            /// Gone (client): Application should set/throw if the referenced
            /// resource has been deleted but could be undeleted. Include
            /// error content.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.11
            GONE,
            LENGTH_REQUIRED, ///< Length required
            /// Precondition Failed (client): Set automatically when
            /// resource does not meet the unmodified constraint.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.13
            PRECONDITION_FAILED,
            /// Request Entity Too Large (client): Application should set/throw if the
            /// request entity is larger than the server is willing or able to process.
            /// The server MAY close the connection to prevent the client from
            /// continuing the request.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.14
            REQUEST_ENTITY_TOO_LARGE,
            REQUEST_URI_TOO_LARGE, ///< Request URI too large
            /// Unsupported Media Type (client): Application should set/throw if the
            /// entity of the request is in a format not supported
            /// by the requested resource for the requested method.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.16
            UNSUPPORTED_MEDIA_TYPE,
            REQUESTED_RANGE_NOT_SATISFIABLE, ///< Requested range not satisfiable
            EXPECTATION_FAILED, ///< Expectation failed
            /// Internal Server Error (server): Set automatically
            /// when any exception without an explicit status code is caught.
            /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.5.1
            INTERNAL_SERVER_ERROR,
            NOT_IMPLEMENTED, ///< Not implemented
            BAD_GATEWAY, ///< Bad gateway
            SERVICE_UNAVAILABLE, ///< Service unavailable
            GATEWAY_TIME_OUT, ///< Gateway time out
            HTTP_VERSION_NOT_SUPPORTED, ///< HTTP version not supported
        };
        status_code_e() : e_(null) {} ///< Construct.
        status_code_e(int code); ///< Construct from integer code, for example 200 is OK.
        status_code_e(e t) : e_(t) {} ///< Construct from enum, for example status_code_e::OK.
        e enumeration() const {return e_;} ///< Get enumeration.
        bool is_null() const {return e_ == null;} ///< Test if null.
        /// cstring representation, including int, for example OK is "200 OK".
        const char* cstring() const {return cstrings_[e_];}
        /// int representation, for example OK is 200.
        operator int() const {return ints_[e_];}
        bool operator ==(const status_code_e& rhs) const {return e_ == rhs.e_;} ///< Equal operator.
        bool operator ==(const status_code_e::e& rhs) const {return e_ == rhs;} ///< Equal operator.
        bool operator <(const status_code_e& rhs) const {return e_ < rhs.e_;} ///< Less operator.
        bool operator <(const status_code_e::e& rhs) const {return e_ < rhs;} ///< Less operator.
        std::ostream& operator <<(std::ostream& os) const; ///< Stream out in HTTP header format.
        static const char EOL_CSTR[3]; ///< end-of-line ("\r\n")
    private:
        e e_;
        static const char* cstrings_[];
        static const int ints_[];
    };
    /// Stream out in HTTP header format.
    inline std::ostream& operator <<(std::ostream& os, const status_code_e& v) {return v.operator <<(os);}
}
#endif
