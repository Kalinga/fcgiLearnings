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
#include "cookie.h"
#include "utils.h"
#include "exception.h"
#include <uripp/utils.h>
#include <stdexcept>
#include <strings.h>
namespace restcgi {
    const char cookie_attrs::SEPARATOR_CSTR[3] = "; ";
    size_t cookie_attrs::VERSION = 1;
    const char cookies::SEPARATOR_CSTR[3] = ", ";
    cookie_attrs::cookie_attrs()
        : max_age_(0), discard_(false), secure_(false), http_only_(false), version_(0) {
    }
    void cookie_attrs::path(const uri_path_type& v) {path_ = v;}
    void cookie_attrs::domain(const std::string& v) {
        if (!v.empty() && (v[0] != '.' || v.size() < 4 || v.find('.', 2) == std::string::npos || v.find(' ', 2) != std::string::npos))
            throw std::invalid_argument("invalid HTTP cookie domain attribute: \"" + v + "\"");
        domain_ = v;
    }
    void cookie_attrs::max_age(size_t v) {
        if ((max_age_ = v))
            discard_ = false;
    }
    void cookie_attrs::discard(bool v) {
        if ((discard_ = v))
            max_age_ = 0;
    }
    void cookie_attrs::secure(bool v) {secure_ = v;}
    void cookie_attrs::http_only(bool v) {http_only_ = v;}
    void cookie_attrs::version(size_t v) {version_ = v;}
    bool cookie_attrs::set(const http_token& name, const http_word& value) {
        try {
            if (!::strcasecmp(name.string().c_str(), "domain"))
                domain_ = value.string();
            else if (!::strcasecmp(name.string().c_str(), "path"))
                path_ = value.string();
            else if (!::strcasecmp(name.string().c_str(), "version"))
                uripp::convert(value.string(), version_);
            else // No other attributes settable by request.
                return false;
        } catch (...) {
            return false;
        }
        return true;
    }
    std::ostream& cookie_attrs::operator <<(std::ostream& os) const {
        if (!domain_.empty())
            os << SEPARATOR_CSTR << "Domain=\"" << domain_ << "\"";
        if (!path_.empty())
            os << SEPARATOR_CSTR << "Path=\"" << path_.encoding() << "\"";
        if (max_age_ || discard_)
            os << SEPARATOR_CSTR << "Max-Age=" << max_age_;
        if (secure_)
            os << SEPARATOR_CSTR << "Secure";
        if (http_only_)
            os << SEPARATOR_CSTR << "HttpOnly";
        if (!comment_.empty())
            os << SEPARATOR_CSTR << "Comment=" << comment_.encoding();
        os << SEPARATOR_CSTR << "Version=" << (version_ ? version_ : VERSION);
        return os;
    }
    cookie::cookie() {}
    cookie::cookie(const http_token& name, const http_word& value, const attrs_type& attrs)
        : name_(name.string()), value_(value), attrs_(attrs)
        , id_(make_id(name_, attrs_)) {
    }
    cookie::cookie(const std::string& name, const std::string& value, const attrs_type& attrs)
        : name_(http_token(name).string()), attrs_(attrs)
        , id_(make_id(name_, attrs_)) {
        value_ += value;
    }
    std::string cookie::make_id(const std::string& name, const attrs_type& attrs) {
        return name + ":" + attrs.domain() + ":" + attrs.path().encoding();
    }
    void cookie::throw_bad_request(const char* what) const {
        throw bad_request(std::string("HTTP header cookie error: ") + name_ + ": " + what);
    }
    std::ostream& cookie::operator <<(std::ostream& os) const {
        return os << name_ << "=" << value_.encoding() << attrs_;
        return os;
    }
    cookies::cookies(bool is_request) : is_request_(is_request), observer_(0) {}
    void cookies::clear() {
        multimap_.clear();
        if (observer_)
            observer_->on_updated(*this);
    }
    cookies::const_iterator cookies::find(const std::string& name) const {return multimap_.find(name);}
    bool cookies::insert(const cookie& c) {
        if (!is_request_)
            for (const_iterator it = begin(); it != end(); ++it)
                if (it->second.id() == c.id())
                    return false;
        multimap_.insert(std::make_pair(c.name(), c));
        if (observer_)
            observer_->on_updated(*this);
        return true;
    }
    bool cookies::parse_version(std::string::const_iterator& first, std::string::const_iterator last, size_t& v) {
        if (httpsyn::parse(first, last, "$")) {
            bool se = false;
            http_token tok;
            if (parse(first, last, tok, false) &&
                !::strcasecmp(tok.string().c_str(), "version") &&
                httpsyn::parse(first, last, "=") &&
                parse(first, last, tok)) { // Found version attr and value.
                try {uripp::convert(tok.string(), v);} catch (...) {
                    se = true; // Syntax error: invalid version number
                }
                if (!se && !httpsyn::parse(first, last, ";") && !httpsyn::parse_eor(first, last))
                    se = true; // Syntax error: missing version attribute termination
            } else
                se = true; // Syntax error: invalid or missing initial version attribute
            if (se && !httpsyn::parse_skip(first, last, ";,")) // Skip to try to sync up with valid info.
                return false;
        }
        return true;
    }
    bool cookies::parse_value(std::string::const_iterator& first, std::string::const_iterator last, http_word& value) {
        if (!httpsyn::parse_any(first, last)) // Syntax error: missing value
            return false;
        if (*first == '\"') { // Quoted string: parse word.
            if (!parse(first, last, value)) { // Syntax error: invalid quoted value
                first = last; // Cannot continue: either end quote missing or corrupt char
                return false;
            }
        } else { // Look for ";, ".
            http_text text;
            parse(first, last, text, ";, ", true); // Must find something before last.
            value += text;
        }
        return true;
    }
    bool cookies::parse_attributes(std::string::const_iterator& first, std::string::const_iterator last, cookie_attrs& attrs) {
        bool syntax_error = false;
        for (; first != last;) {
            bool se = false;
            // Find previous token (n-v or attr) terminator.
            if (!httpsyn::parse_any(first, last)) // No chars left.
                break;
            if (*first == ',') { // End of attrs.
                // Consume ',' to be consistent with consumption of ';'.
                ++first;
                break;
            }
            if (*first != ';') // Not an attribute start.
                break;
            if (!httpsyn::parse_any(++first, last)) // No chars left.
                break;
            if (*first != '$') // Not an attribute name.
                break;
            http_token name;
            http_word value;
            if (!parse(++first, last, name, false))
                se = true; // Syntax error: invalid attribute name
            else if (!httpsyn::parse(first, last, "="))
                se = true; // Syntax error: missing attribute "="
            else if (!parse_value(first, last, value))
                se = true; // Syntax error: invalid attribute value
            else if (!attrs.set(name, value))
                se = true; // Syntax error: unknown or invalid attribute
            if (se) { // Try to sync up.
                if (!httpsyn::parse_skip(first, last, ";,"))
                    return false; // Cannot continue: nowhere to sync.
                syntax_error = true;
            }
        }
        return !syntax_error;
    }
    bool cookies::insert(const std::string& v) {
        std::string::const_iterator first = v.begin();
        std::string::const_iterator last = v.end();
        size_t version = 0;
        if (!parse_version(first, last, version)) // Optional version spec.
            return false;
        bool syntax_error = false;
        for (; first != last;) { // cookie-value(s)
            bool se = false;
            // Name token.
            http_token name;
            if (parse(first, last, name)) {
                if (name.string()[0] == '$')
                    se = true; // Syntax error: missing attribute separator
            } else {
                if (httpsyn::parse_eor(first, last)) // Only spaces left.
                    break;
                se = true; // Syntax error: missing name or extra chars in cookie
            }
            if (!se && !httpsyn::parse(first, last, "="))
                se = true; // Syntax error: missing "="
            if (!se) {
                // Value.
                http_word value;
                cookie_attrs attrs;
                attrs.version(version);
                if (parse_value(first, last, value) && parse_attributes(first, last, attrs))
                    // Good cookie.
                    insert(cookie(name, value, attrs));
                else
                    se = true;
            }
            if (se) { // Error: skip to try to sync up with valid info.
                if (!httpsyn::parse_skip(first, last, ";,"))
                    return false; // Cannot continue: nowhere to sync.
                syntax_error = true;
            }
        }
        return !syntax_error;
    }
    std::ostream& cookies::operator <<(std::ostream& os) const {
        bool first = true;
        for (const_iterator it = begin(); it != end(); ++it) {
            if (first)
                first = false;
            else
                os << SEPARATOR_CSTR;
            os << it->second;
        }
        return os;
    }
    void cookies::attach(observer* o) {observer_ = o;}
    void cookies::detach(observer* o) {
        if (observer_ == o)
            observer_ = 0;
    }
}
