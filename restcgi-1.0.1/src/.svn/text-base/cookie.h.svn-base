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
#ifndef restcgi_cookie_h
#define restcgi_cookie_h
#include "apidefs.h"
#include "httpsyn.h"
#include "utils.h"
#include <uripp/utils.h>
#include <uripp/path.h>
#include <string>
#include <map>
#include <iostream>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    /** \brief Cookie attributes.
     * @see cookie */
    struct RESTCGI_API cookie_attrs {
        typedef uripp::path uri_path_type; ///< URI path type
        cookie_attrs(); ///< Construct with default values and full security.
        /// Domain with which to match for returning cookie. Must
        /// begin with a dot and contain another dot. The cookie
        /// will be rejected if the string resulting from subtracting
        /// this string as a suffix from the request-host domain
        /// contains a dot. In other words, you can only widen
        /// the domain applicability of the cookie by at most one level.
        /// Defaults to the request-host (no leading dot, exact match).
        const std::string& domain() const {return domain_;}
        /// Set domain.
        /// @exception std::invalid_argument if not valid.
        void domain(const std::string& v);
        /// Path with which to match for returning cookie.
        /// Must be a prefix of the request URI path.
        /// Defaults to the path of the request URI that generated
        /// the Set-Cookie response, up to, but not including, the
        /// right-most '/'.
        const uri_path_type& path() const {return path_;}
        void path(const uri_path_type& v); ///< Set path.
        /// The lifetime of the cookie in seconds. Default is to
        /// discard the associated cookie when the user agent exits.
        /// @see discard
        size_t max_age() const {return max_age_;}
        /// Set max age, and if not zero reset discard.
        void max_age(size_t v);
        /// Discard the cookie immediately. Use this instead of
        /// setting the max_age to 0. Defaults to not discard
        /// immediately (false), i.e. use max_age, if set, or discard
        /// when user agent exits. (Note "discard" is also the
        /// name of a RFC2965 attribute that has the same effect.
        /// Here, <code>Discard</code> is not sent explicitly, rather,
        /// it causes <code>Max-Age=0</code> to be sent.)
        bool discard() const {return discard_;}
        /// Set discard, and if true set max age to 0.
        void discard(bool v);
        /// Send only over secure channel. Defaults to not secure channel
        /// only (false).
        bool secure() const {return secure_;}
        void secure(bool v); ///< Set secure.
        /// Cookie is for HTTP only, do not allow scripts
        /// to access. Defaults to false. (Microsoft IE 6 SP1 feature but
        /// now supported by other browsers.)
        /// @see http://msdn2.microsoft.com/en-us/library/ms533046.aspx
        bool http_only() const {return http_only_;}
        void http_only(bool v);  ///< Set HTTP only.
        /// Comment for user agent only, not returned, no side-effects.
        const http_word& comment() const {return comment_;}
        void comment(const http_word& v) {comment_ = v;} ///< Set comment.
        /// Version, defaults to 1.
        size_t version() const {return version_;}
        void version(size_t v); ///< Set version.
        /// Set attribute with name to value, returning whether
        /// attribute exists and value is valid. This is for use
        /// when setting cookies in a request (e.g. domain string
        /// constraints not enforced).
        bool set(const http_token& name, const http_word& value);
        /// Stream out in HTTP header format.
        std::ostream& operator <<(std::ostream& os) const;
        static const char SEPARATOR_CSTR[3]; ///< separator cstr ("; ")
        static size_t VERSION; ///< version (1)
    private:
        std::string domain_;
        uri_path_type path_;
        size_t max_age_;
        bool discard_;
        bool secure_;
        bool http_only_;
        http_word comment_;
        size_t version_;
    };
    /** \brief Stream out in HTTP header (response) format. */
    inline std::ostream& operator <<(std::ostream& os, const cookie_attrs& v) {return v.operator <<(os);}
    /** \brief HTTP cookie as a name, value, and attributes.
     *
     * When creating a cookie (responses) the name is forced to be
     * a strict HTTP "token" and value a HTTP "word". This is relaxed
     * on parsing as browsers and JavaScripts have different formats.
     *
     * Follows RFC2109 standard. Here are the differences between
     * Netscape, RFC2109, and RFC2965:<ol>
     * <li>Netscape allows only 1 n-v pair per Set-Cookie (multiple hdr lines),
     *     RFCs allow many (1 hdr line).</li>
     * <li>Netscape Set-Cookie has "expires", RFCs "Max-Age". Note that
     *     "expires" date is not quoted.</li>
     * <li>Netscape Cookie separates n-v pairs with semi-colons,
     *     RFCs with commas (although it advises to accept both). </li>
     * <li>Netscape does not send attributes with Cookie, RFCs do.</li>
     * <li>RFC2109 uses "Set-Cookie" and "Cookie", RFC2965 "Set-Cookie2"
     *     and "Cookie2".</li>
     * <li>Other attribute differences.</li>
     * </ol>
     * @see cookies, cookie_attrs
     * @see http://wp.netscape.com/newsref/std/cookie_spec.html
     * @see http://tools.ietf.org/html/rfc2109
     * @see http://tools.ietf.org/html/rfc2965 */
    class RESTCGI_API cookie {
    public:
        typedef restcgi::cookie_attrs attrs_type; ///< cookie attrs type
        cookie(); ///< Construct null.
        /// Construct.
        cookie(const http_token& name, const http_word& value, const attrs_type& attrs = attrs_type());
        /// Construct.
        /// @exception std::invalid_argument if name or value are not valid
        cookie(const std::string& name, const std::string& value, const attrs_type& attrs = attrs_type());
        bool is_null() const {return name_.empty();} ///< Test if null.
        const std::string& name() const {return name_;} ///< Get name.
        /// Get converted, unencoded value.
        /// @exception bad_request on conversion error
        template <typename T> T value() const {
            T v;
            try {uripp::convert(value_.string(), v);}
            catch (const std::exception& e) {throw_bad_request(e.what());}
            return v;
        }
        const attrs_type& attrs() const {return attrs_;} ///< Get attrs.
        const std::string& id() const {return id_;} ///< Get id made up of name:domain:path.
        /// Stream out in HTTP header format.
        std::ostream& operator <<(std::ostream& os) const;
    private:
        void throw_bad_request(const char* what) const;
        static std::string make_id(const std::string& name, const attrs_type& attrs);
        std::string name_;
        http_word value_;
        attrs_type attrs_;
        std::string id_;
    };
    /** \brief Stream out in HTTP header (response) format. */
    inline std::ostream& operator <<(std::ostream& os, const cookie& v) {return v.operator <<(os);}
    /** \brief Collection of cookies.
     *
     * This is a multimap (name is key) of cookies
     * that is used for both requests and responses. For requests the
     * insert(const std::string&) method is called automatically by general_hdr,
     * for responses the application must call insert to create a cookie.
     *
     * When inserting (for response) there is a constraint that the
     * cookie ids must be unique. The general_hdr "Set-Cookie" field
     * is automatically updated with each insert.
     *
     * Follows RFC2109 standard, but parses Netscape as well (see insert).
     * @see cookie, general_hdr */
    class RESTCGI_API cookies {
    public:
        typedef std::multimap<std::string, cookie> multimap_type; ///< multimap type
        typedef multimap_type::const_iterator const_iterator; ///< iterator
        cookies(bool is_request); ///< Construct.
        bool empty() const {return multimap_.empty();} ///< Test if empty.
        void clear(); ///< Clear all cookies.
        /// Find start of cookies with given name, returning end() if none.
        const_iterator find(const std::string& name) const;
        /// Insert cookie, returning whether the id is new/cookie inserted.
        bool insert(const cookie& c);
        /// Parse the "Cookie" header string into individual cookies
        /// and insert into collection. RFC2109 syntax almost supports Netscape.
        /// This uses a modified syntax to allow Netscape by not
        /// requiring a cookie-version and not forcing VALUE to be quoted-text.
        /// Note that the cookies are always inserted, if found, whether the
        /// whole string completely parses or not, which is the returned indication.
        ///
        /// RFC2109 modified syntax:
        /// <pre>
        /// cookies      = [cookie-ver ";"] cookie-value *((";" | ",") cookie-value)
        /// cookie-value = NAME "=" VALUE [";" path] [";" domain]
        /// cookie-ver   = "$Version" "=" value
        /// NAME         = attr
        /// VALUE        = \<value or up to ";" or ",">
        /// path         = "$Path" "=" value
        /// domain       = "$Domain" "=" value
        /// </pre>
        bool insert(const std::string& v);
        /// Stream out in HTTP header format.
        std::ostream& operator <<(std::ostream& os) const;
        const_iterator begin() const {return multimap_.begin();} ///< Get multimap beginning.
        const_iterator end() const {return multimap_.end();} ///< Get multimap end.
        /** \brief Cookies observer interface.
         *
         * Used to keep hdr in synch with cookie. */
        struct observer {virtual ~observer() {} /** Updated. */ virtual void on_updated(const cookies& v) = 0;};
        void attach(observer* o); ///< Attach observer (must outlive this).
        void detach(observer* o); ///< Detach observer.
        static const char SEPARATOR_CSTR[3]; ///< separator cstr (", ")
    private:
        static bool parse_version(std::string::const_iterator& first, std::string::const_iterator last, size_t& v);
        static bool parse_value(std::string::const_iterator& first, std::string::const_iterator last, http_word& value);
        static bool parse_attributes(std::string::const_iterator& first, std::string::const_iterator last, cookie_attrs& attrs);
        multimap_type multimap_;
        bool is_request_;
        observer* observer_;
    };
    /** \brief Stream out in HTTP header (response) format. */
    inline std::ostream& operator <<(std::ostream& os, const cookies& v) {return v.operator <<(os);}
}
#endif
