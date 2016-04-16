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
#include "exception.h"
namespace restcgi {
    const char* exception::VARIABLE_NAME_PREFIX = "exception_";
    exception::exception(const status_code_e& sc, const std::string& errmsg) throw()
        : status_code_(sc), errmsg_(errmsg) {
        what_ = errmsg.empty() ? std::string(sc.cstring()) : errmsg;
        exception_insert("status_code", status_code_.cstring());
        exception_insert("errmsg", errmsg_);
        exception_insert("what", what_);
    }
    exception::~exception() throw() {}
    bool exception::insert(const std::string& name, const std::string& value) {
        return map_.insert(std::make_pair(name, value)).second;
    }
    bool exception::exception_insert(const std::string& name, const std::string& value) {
        return insert(VARIABLE_NAME_PREFIX + name, value);
    }
    const status_code_e no_content::STATUS_CODE = status_code_e::NO_CONTENT;
    no_content::no_content() throw() : exception(STATUS_CODE, "successful, no content") {}
    no_content::~no_content() throw() {}
    const status_code_e reset_content::STATUS_CODE = status_code_e::RESET_CONTENT;
    reset_content::reset_content() throw() : exception(STATUS_CODE, "successful, reset content") {}
    reset_content::~reset_content() throw() {}
    const status_code_e see_other::STATUS_CODE = status_code_e::SEE_OTHER;
    see_other::see_other(const std::string& location) throw()
        : exception(STATUS_CODE, "see other resource"), location_(location) {
        exception_insert("location", location_);
    }
    see_other::~see_other() throw() {}
    void see_other::copy_hdr_flds(map_type& map) const {map.insert(std::make_pair("location", location_));}
    const status_code_e not_modified::STATUS_CODE = status_code_e::NOT_MODIFIED;
    not_modified::not_modified(const std::string& etag) throw()
        : exception(STATUS_CODE, "document has not been modified"), etag_(etag) {
        exception_insert("etag", etag_);
    }
    not_modified::~not_modified() throw() {}
    void not_modified::copy_hdr_flds(map_type& map) const {
        if (!etag_.empty())
            map.insert(std::make_pair("etag", etag_));
    }
    const status_code_e bad_request::STATUS_CODE = status_code_e::BAD_REQUEST;
    bad_request::bad_request(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg) {
    }
    const status_code_e unauthorized::STATUS_CODE = status_code_e::UNAUTHORIZED;
    unauthorized::unauthorized(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg) {
    }
    const status_code_e not_found::STATUS_CODE = status_code_e::NOT_FOUND;
    not_found::not_found(const std::string& uri_path_rem) throw()
        : exception(STATUS_CODE), uri_path_rem_(uri_path_rem) {
        exception_insert("uri_path_rem", uri_path_rem_);
    }
    not_found::~not_found() throw() {}
    const status_code_e method_not_allowed::STATUS_CODE = status_code_e::METHOD_NOT_ALLOWED;
    method_not_allowed::method_not_allowed(const std::string& allow) throw()
        : exception(STATUS_CODE, "requested method is not allowed by resource"), allow_(allow) {
        exception_insert("allow", allow_);
    }
    method_not_allowed::~method_not_allowed() throw() {}
    void method_not_allowed::copy_hdr_flds(map_type& map) const {
        if (!allow_.empty())
            map.insert(std::make_pair("allow", allow_));
    }
    const status_code_e conflict::STATUS_CODE = status_code_e::CONFLICT;
    conflict::conflict(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg) {
    }
    const status_code_e gone::STATUS_CODE = status_code_e::GONE;
    gone::gone(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg.empty() ? std::string("resource is no longer available and address is not known") : errmsg) {
    }
    const status_code_e precondition_failed::STATUS_CODE = status_code_e::PRECONDITION_FAILED;
    precondition_failed::precondition_failed(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg) {
    }
    const status_code_e request_entity_too_large::STATUS_CODE = status_code_e::REQUEST_ENTITY_TOO_LARGE;
    request_entity_too_large::request_entity_too_large(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg) {
    }
    const status_code_e unsupported_media_type::STATUS_CODE = status_code_e::UNSUPPORTED_MEDIA_TYPE;
    unsupported_media_type::unsupported_media_type(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg.empty() ? std::string("unsupported media type") : errmsg) {
    }
    const status_code_e internal_server_error::STATUS_CODE = status_code_e::INTERNAL_SERVER_ERROR;
    internal_server_error::internal_server_error(const std::string& errmsg) throw()
        : exception(STATUS_CODE, errmsg) {
    }
}
