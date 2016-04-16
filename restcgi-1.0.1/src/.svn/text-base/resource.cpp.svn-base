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
#include "resource.h"
#include "method.h"
#include "env.h"
#include "exception.h"
#include <stdexcept>
namespace restcgi {
    uri_info::uri_info() {}
    uri_info::uri_info(method_pointer m) : method_(m) {}
    void uri_info::method(method_pointer m) {method_ = m;}
    const uripp::path& uri_info::path() const {return method_->uri_path();}
    const uripp::query& uri_info::query() const {return method_->uri_query();}
    resource::resource(int methods_allowed_mask, bool chain_children)
        : methods_allowed_mask_(methods_allowed_mask), chain_children_(chain_children), default_response_(false) {
    }
    resource::~resource() {}
    void resource::default_response(bool v) {default_response_ = v;}
    void resource::child(pointer rthis, pointer cr) {
        child_ = cr;
        cr->parent_ = rthis;
    }
    resource::pointer resource::locate(uri_path_type& path) {
        // Apply method to this (if path is not empty bad_request will result).
        return pointer();
    }
    void resource::respond(const version_type& v) {
        status_code_e sc = status_code_e::OK;
        response_hdr rh;
        copy(v, rh);
        if (default_response_) { // Resource does not want to be called.
            default_response_ = false;
            method_->respond(sc, rh);
        } else {
            content_hdr ch;
            on_responding(sc, rh, ch);
            ocontent::pointer oc = method_->respond(ch, sc, rh);
            write(oc);
        }
    }
    void resource::get_method() {
        setup();
        version_type v = read(false);
        version_constraint_.assert_satisfied(v, me());
        respond(v);
    }
    void resource::put_method() {
        setup();
        version_type vold = read(false);
        version_constraint_.assert_satisfied(vold, me());
        version_type vnew = update(method_->icontent());
        respond(vnew);
    }
    void resource::post_method() {
        setup();
        pointer cr = create_child();
        if (!parent_.expired() && !child_)
            child(parent()->child(), cr);
        cr->method(method_);
        cr->setup();
        id_version_type iv = cr->create(method_->icontent());
        status_code_e sc = status_code_e::CREATED;
        response_hdr rh;
        copy(iv.second, rh);
        if (!iv.first.empty())
            rh.content_location((uri_info_.path() + iv.first).encoding());
        if (cr->default_response_) {
            cr->default_response_ = false;
            method_->respond(sc, rh);
        } else {
            content_hdr ch;
            cr->on_responding(sc, rh, ch);
            ocontent::pointer oc = method_->respond(ch, sc, rh);
            cr->write(oc);
        }
    }
    void resource::del_method() {
        setup();
        if (!version_constraint_.is_null()) {
            version_type v = read(true);
            version_constraint_.assert_satisfied(v, me());
        }
        del();
        respond();
    }
    void resource::options_method() {
        setup();
        status_code_e sc = status_code_e::OK;
        response_hdr rh;
        rh.allow(method_e::mask_to_string(methods_allowed_mask_));
        content_hdr ch;
        ch.content_length(0);
        on_responding(sc, rh, ch);
        method_->respond(ch, sc, rh);
    }
    void resource::head_method() {
        setup();
        // Do what GET would do but w/o write.
        version_type v = read(false);
        version_constraint_.assert_satisfied(v, me());
        status_code_e sc = status_code_e::OK;
        response_hdr rh;
        copy(v, rh);
        content_hdr ch;
        on_responding(sc, rh, ch);
        method_->respond(ch, sc, rh);
    }
    void resource::method(method_pointer m) {method_ = m;}
    void resource::setup() {
        uri_info_.method(method_);
        copy(method_->request_hdr(), version_constraint_);
    }
    const method_e& resource::me() const {
        if (!method_)
            throw std::domain_error("method not set on resource");
        return method_->e();
    }
    resource::version_type resource::read(bool veronly) {
        throw internal_server_error("resource::read not implemented by application");
    }
    resource::version_type resource::update(icontent::pointer ic) {
        throw internal_server_error("resource::update not implemented by application");
    }
    resource::pointer resource::create_child() {
        throw internal_server_error("resource::create_child not implemented by application");
    }
    resource::id_version_type resource::create(icontent::pointer ic) {
        throw internal_server_error("resource::create not implemented by application");
    }
    void resource::del() {
        throw internal_server_error("resource::del not implemented by application");
    }
    void resource::on_responding(status_code_e& sc, response_hdr& rh, content_hdr& ch) {}
    void resource::write(ocontent::pointer oc) {}
    void copy(const request_hdr& rh, version_constraint& vc) {
        // Note that the RFC says that the behavior is undefined if
        // conflicting constraints are given. We choose the stronger constraint.
        std::string matchtag = rh.if_match();
        if (matchtag == "*") // "*" handling: see class description.
            matchtag.clear();
        date_time unmodsince;
        // Ignore if bad format.
        try {rh.if_unmodified_since(unmodsince, date_time());} catch (...) {}
        date_time now = date_time::now();
        if (!unmodsince.is_null() && now < unmodsince) // Invalid if future.
            unmodsince = date_time();
        std::string nonetag = rh.if_none_match(); // "*" handling: see class description.
        date_time modsince;
        // Ignore if bad format.
        try {rh.if_modified_since(modsince, date_time());} catch (...) {}
        if (!modsince.is_null() && now < modsince) // Invalid if future.
            modsince = date_time();
        // Test and set from strongest to weakest, only pairing tag
        // and date-time constraints that go together.
        if (!matchtag.empty())
            vc = version_constraint(version(version_tag(matchtag), unmodsince), false);
        else if (!unmodsince.is_null())
            vc = version_constraint(version(unmodsince), false);
        else if (!nonetag.empty())
            vc = version_constraint(version(version_tag(nonetag), modsince), true);
        else if (!modsince.is_null())
            vc = version_constraint(version(modsince), true);
    }
    void copy(const version& v, response_hdr& rh) {
        if (!v.tag().is_null())
            rh.etag(v.tag().string());
        if (!v.date_time().is_null())
            rh.last_modified(v.date_time());
    }
}
