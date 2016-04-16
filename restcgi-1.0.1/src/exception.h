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
#ifndef restcgi_exception_h
#define restcgi_exception_h
#include "apidefs.h"
#include "status_code_e.h"
#include <string>
#include <map>
#include <exception>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    /** \brief Base class for throwing exceptions when processing HTTP methods.
     *
     * This has information about the HTTP status code so that
     * exception responses can be automatically generated, see ctmpl.
     * Note that not all exceptions are considered "errors", per se
     * (e.g. not modified).
     *
     * This contains a map of name-value string pairs. Member data of
     * derived classes are inserted into the map with the name the same
     * as the accessor method with "exception_" prefixed,
     * e.g. not_found has a uri_path_rem() accessor method
     * which will store the value under "exception_uri_path_rem".
     * Application code can also store arbitrary variables for
     * passing to the content template.
     *
     * Special variables are inserted into the map by the base
     * exception class:
     * <ul><li>exception_status_code: e.g. "304 Not Modified"</li>
     * <li>exception_errmsg: error message string. Note this is not
     * set by all exceptions.</li>
     * <li>exception_what: the what() string (status code plus errmsg)</li></ul>
     * @see status_code_e, ctmpl */
    class RESTCGI_API exception : public std::exception {
    public:
        typedef std::map<std::string, std::string> map_type; ///< map type
        /// Construct. Note the dervided classes, e.g.
        /// not_found, set the arguments automatically.
        exception(const status_code_e& sc, const std::string& errmsg = "") throw();
        ~exception() throw();
        const status_code_e& status_code() const throw() {return status_code_;} ///< Get status code.
        /// Get what string. This is the status code, e.g. "304 Not Modified",
        /// plus an error message (preceded by a ": "), if any.
        const char* what() const throw() {return what_.c_str();}
        const std::string& errmsg() const {return errmsg_;} ///< Get errmsg.
        /// Inhibit content? Some status codes prohibit the sending of entity
        /// content, this indicates exception is for such a status code (e.g. NOT_MODIFIED).
        virtual bool inhibit_content() const {return false;}
        const map_type& map() const {return map_;} ///< Get const map.
        map_type& map() {return map_;} ///< Get map.
        /// Insert name and value into map if name not in map,
        /// returning whether inserted. Application code can use this,
        /// before throwing the exception, for passing variable values
        /// to the content template (ctmpl).
        bool insert(const std::string& name, const std::string& value);
        virtual void copy_hdr_flds(map_type& map) const {} ///< Copy hdr field name and values.
        static const char* VARIABLE_NAME_PREFIX; ///< variable name prefix ("exception_")
    protected:
        bool exception_insert(const std::string& name, const std::string& value); ///< Prefix name and insert with value.
    private:
        status_code_e status_code_;
        std::string errmsg_;
        std::string what_;
        map_type map_;
    };
    /** \brief "No content" exception.
     *
     * Note: output content not allowed.
     * @see status_code_e::NO_CONTENT */
    class RESTCGI_API no_content : public exception {
    public:
        no_content() throw(); ///< Construct.
        ~no_content() throw();
        bool inhibit_content() const {return true;} ///< Inhibit content?
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Reset content" exception.
     *
     * Note: output content not allowed.
     * @see status_code_e::RESET_CONTENT */
    class RESTCGI_API reset_content : public exception {
    public:
        reset_content() throw(); ///< Construct.
        ~reset_content() throw();
        bool inhibit_content() const {return true;} ///< Inhibit content?
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "See other" exception.
     * @see status_code_e::SEE_OTHER */
    class RESTCGI_API see_other : public exception {
    public:
        /// Construct with location.
        see_other(const std::string& location) throw();
        ~see_other() throw();
        void copy_hdr_flds(map_type& map) const; ///< Copy hdr field name and values.
        const std::string& location() const {return location_;} ///< Get location.
        static const status_code_e STATUS_CODE; ///< status code for exception
    private:
        std::string location_;
    };
    /** \brief "Not modified" exception.
     *
     * Note: output content not allowed.
     * @see status_code_e::NOT_MODIFIED */
    class RESTCGI_API not_modified : public exception {
    public:
        /// Construct, optionally with etag.
        not_modified(const std::string& etag = "") throw();
        ~not_modified() throw();
        bool inhibit_content() const {return true;} ///< Inhibit content?
        void copy_hdr_flds(map_type& map) const; ///< Copy hdr field name and values.
        const std::string& etag() const {return etag_;} ///< Get etag.
        static const status_code_e STATUS_CODE; ///< status code for exception
    private:
        std::string etag_;
    };
    /** \brief "Bad request" exception.
     * @see status_code_e::BAD_REQUEST */
    class RESTCGI_API bad_request : public exception {
    public:
        /// Construct with errmsg.
        bad_request(const std::string& errmsg) throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Unauthorized" exception.
     * @see status_code_e::UNAUTHORIZED */
    class RESTCGI_API unauthorized : public exception {
    public:
        /// Construct with errmsg.
        unauthorized(const std::string& errmsg) throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Not found" exception.
     * @see status_code_e::NOT_FOUND */
    class RESTCGI_API not_found : public exception {
    public:
        /// Construct with remainder of path not found.
        not_found(const std::string& uri_path_rem) throw();
        ~not_found() throw();
        /// Get remainder of path, the first segment of which
        /// should be what caused the exception.
        const std::string& uri_path_rem() const {return uri_path_rem_;}
        static const status_code_e STATUS_CODE; ///< status code for exception
    private:
        std::string uri_path_rem_;
    };
    /** \brief "Method not allowed" exception.
     * @see status_code_e::METHOD_NOT_ALLOWED */
    class RESTCGI_API method_not_allowed : public exception {
    public:
        /// Construct giving the allowed methods as a mask of method_e enumerations.
        method_not_allowed(const std::string& allow) throw();
        ~method_not_allowed() throw();
        void copy_hdr_flds(map_type& map) const; ///< Copy hdr field name and values.
        const std::string& allow() const {return allow_;} ///< Get methods allowed, for example "GET, PUT".
        static const status_code_e STATUS_CODE; ///< status code for exception
    private:
        std::string allow_;
    };
    /** \brief "Conflict" exception.
     * @see status_code_e::CONFLICT */
    class RESTCGI_API conflict : public exception {
    public:
        /// Construct with errmsg.
        conflict(const std::string& errmsg) throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Gone" exception.
     * @see status_code_e::GONE */
    class RESTCGI_API gone : public exception {
    public:
        /// Construct with optional, replacement errmsg.
        gone(const std::string& errmsg = "") throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Precondition failed" exception.
     * @see status_code_e::PRECONDITION_FAILED */
    class RESTCGI_API precondition_failed : public exception {
    public:
        /// Construct with errmsg.
        precondition_failed(const std::string& errmsg) throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Request Entity Too Large" exception.
     * @see status_code_e::REQUEST_ENTITY_TOO_LARGE */
    class RESTCGI_API request_entity_too_large : public exception {
    public:
        /// Construct with errmsg.
        request_entity_too_large(const std::string& errmsg) throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Unsupported Media Type" exception.
     * @see status_code_e::UNSUPPORTED_MEDIA_TYPE */
    class RESTCGI_API unsupported_media_type : public exception {
    public:
        /// Construct with optional, replacement errmsg.
        unsupported_media_type(const std::string& errmsg = "") throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
    /** \brief "Internal server error" exception.
     * @see status_code_e::INTERNAL_SERVER_ERROR */
    class RESTCGI_API internal_server_error : public exception {
    public:
        /// Construct with optional, replacement errmsg.
        internal_server_error(const std::string& errmsg = "") throw();
        static const status_code_e STATUS_CODE; ///< status code for exception
    };
}
#endif
