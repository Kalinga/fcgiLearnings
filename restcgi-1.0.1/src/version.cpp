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
#include "version.h"
#include "utils.h"
#include "exception.h"
#include <uripp/utils.h>
namespace restcgi {
    version_tag::version_tag() {}
    version_tag::version_tag(const std::string& v) : string_(v) {}
    version_tag::version_tag(int v) : string_(uripp::convert(v)) {}
    version_tag::version_tag(unsigned int v) : string_(uripp::convert(v)) {}
    version_tag::version_tag(long long v) : string_(uripp::convert(v)) {}
    version_tag::version_tag(unsigned long long v) : string_(uripp::convert(v)) {}
    version::version() {}
    version::version(const tag_type& tag, const date_time_type& dt) : tag_(tag), date_time_(dt) {}
    version::version(const date_time_type& dt, const tag_type& tag) : tag_(tag), date_time_(dt) {}
    version_constraint::version_constraint() : modified_(false) {}
    version_constraint::version_constraint(const version_type& v, bool mod)
        : version_(v), modified_(mod) {
    }
    void version_constraint::assert_satisfied(const version_type& v, const method_e& e) const {
        if (modified_) { // Version must be more recent.
            // If-None-Match and If-Modified-Since hdr fields.
            // Tag constraint has precedence.
            if (!version_.tag().is_null() && !v.tag().is_null()) {
                // Throw if tags are equal and there is no date-time info
                // or the version is the same or older. In other words,
                // a date-time modification will override a tag failure.
                if ((version_.tag().string() == "*" || version_.tag() == v.tag()) &&
                    (version_.date_time().is_null() || v.date_time().is_null() ||
                     v.date_time() <= version_.date_time())) {
                    if (e == method_e::GET || e == method_e::HEAD)
                        throw not_modified(v.tag().string());
                    throw precondition_failed("resource tag is the same (and modified date is not given or is same or older): " + v.tag().string());
                }
            } else if (!version_.date_time().is_null() && !v.date_time().is_null() &&
                       v.date_time() <= version_.date_time())
                // Modified date-time is the same or older.
                throw not_modified(v.tag().string());
        } else { // Version must be the same.
            // If-Match and If-Unmodified-Since hdr fields.
            if (!version_.tag().is_null() && !v.tag().is_null() && version_.tag() != v.tag())
                // Tags are not equal.
                throw precondition_failed("resource tag is different: " + v.tag().string());
            if (!version_.date_time().is_null() && !v.date_time().is_null() &&
                version_.date_time() < v.date_time())
                // Modified date-time is more recent.
                throw precondition_failed("resource modified date is more recent: " + v.date_time().string());
        }
    }
}
