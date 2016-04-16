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
#ifndef restcgi_hdr_h
#define restcgi_hdr_h
#include "apidefs.h"
#include "cookie.h"
#include "utils.h"
#include <string.h>
#include <string>
#include <map>
#include <iostream>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    class env;
    class date_time;
    class request_hdr;
    class content_hdr;
    /** \brief HTTP header base class mapping field names to values.
     *
     * Notes:<ul>
     * <li>Each standard HTTP field is categorized by RFC 2616 (plus
     *     2109 for cookies).</li>
     * <li>The concrete derived classes are request_hdr, response_hdr,
     *     and content_hdr. The request and response hdrs both contain
     *     general and cookie fields in addition to those of their own
     *     category. The entity fields have been split between response
     *     and content (to put closer to the actual content stream).</li>
     * <li>A field's accessor is defined in the class for its respective
     *     category (with the exception of some of the entity fields
     *     as noted above). The accessor is named the same as the field
     *     but with '_' instead of '-' and lower cased. For example, the
     *     "Content-Type" field accessor is content_type().</li>
     * <li>When calling the read accessor of a field for a non-string
     *     value, e.g. content_hdr::content_length, (or find) the
     *     conversion is automatically performed and a
     *     bad_request exception is thrown on conversion errors.
     *     The methods return whether or not the field is in the hdr
     *     and will set a default value if not.</li>
     * <li>There is a mechanism for inserting and finding non-standard
     *     fields that uses the name string for identification.
     *     The string is treated as case-insensitive for lookups but
     *     is sent as on insert.</li>
     * <li>Cookies have their own class with insert and parsing
     *     functionality. The general_hdr includes cookies and keeps
     *     the cookies in sync with the actual header fields.</li>
     * </ul>
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html */
    class RESTCGI_API hdr {
    public:
        /// Field category enumeration. IN TRANSMISSION ORDER!!!
        enum fld_category {
            fc_null, ///< none
            fc_general = 1<<0, ///< general fields
            fc_request = 1<<1, ///< request fields
            fc_response = 1<<2, ///< response fields
            fc_other = 1<<3, ///< other fields
            fc_entity_rsp = 1<<4, ///< entity response fields
            fc_entity_cnt = 1<<5, ///< entity content fields
        };
        /// Traits for the standard fields.
        class RESTCGI_API fld_traits {
        public:
            bool operator <(const fld_traits& rhs) const {return ln_ ? (rhs.ln_ ? (::strcmp(ln_, rhs.ln_) < 0) : false) : (rhs.ln_ != 0);} ///< less
            const char* lower_name_as_cstring() const {return ln_;} ///< lower name as cstring, e.g. "content-type"
            const char* name_as_cstring() const {return n_;} ///< name as cstring, e.g. "Content-Type"
            fld_category category() const {return category_;} ///< category
        private:
            friend class hdr;
            fld_traits(const char* ln, const char* n = 0, fld_category cat = fc_null);
            const char* ln_;
            const char* n_;
            fld_category category_;
        };
        /** \brief Key for field-to-value map.
         *
         * This can be either a standard field traits pointer or "other" field name. */
        class RESTCGI_API key {
        public:
            key(const std::string& name, bool lookup = false); ///< construct
            bool is_null() const {return !p_ && name_.empty();} ///< test is null
            bool operator ==(const key& rhs) const {return p_ == rhs.p_ && lower_name_ == rhs.lower_name_;} ///< equal
            bool operator <(const key& rhs) const; ///< less, sort by category then name
            const char* name_as_cstring() const {return p_ ? p_->name_as_cstring() : name_.c_str();} ///< name as cstring
            const char* lower_name_as_cstring() const {return p_ ? p_->lower_name_as_cstring() : lower_name_.c_str();} ///< lower name as cstring
            fld_category category() const {return p_ ? p_->category() : fc_other;} ///< category
        private:
            friend class hdr;
            key(const fld_traits* p = 0) : p_(p) {}
            const fld_traits* p_; ///< if standard
            std::string name_; ///< if not standard ("other")
            std::string lower_name_; ///< lower case if not standard ("other")
        };
        typedef std::map<key, std::string> map_type; ///< map type
        typedef map_type::const_iterator const_iterator; ///< const iterator
        virtual ~hdr(); ///< Destruct.
        bool empty() const {return map_.empty();} ///< Test if empty.
        const_iterator begin() const {return map_.begin();} ///< Get map beginning.
        const_iterator end() const {return map_.end();} ///< Get map end.
        /// Insert the field into the header, returning whether it
        /// was inserted or not. Fields are not overwritten if already
        /// in the header or the insert value is empty. Works for both
        /// standard fields and "other" fields. The name
        /// is case insensitive on lookup.
        /// @return false if exists, invalid category or name or value empty
        template<typename T> bool insert(const std::string& name, const T& value) {
            if (name.empty())
                return false;
            key k(name, true);
            std::string v = uripp::convert(value);
            return insert(k, v);
        }
        /// Find the field returning the typed value and whether found.
        /// Works with both standard and "other" fields. The name
        /// is case insensitive on lookup.
        /// @exception bad_request if conversion error
        template<typename T> bool find(const std::string& name, T& value) const {
            if (name.empty())
                return false;
            key k(name, true);
            const_iterator it = map_.find(k);
            if (it == end())
                return false;
            try {uripp::convert(it->second, value);}
            catch (const std::exception& e) {throw_bad_request(k.name_as_cstring(), e.what());}
            return true;
        }
        bool erase(const std::string& name); ///< Erase field, returning whether successful.
        /// Stream out in HTTP header format.
        std::ostream& operator <<(std::ostream& os) const;
        /// Find standard field traits from lower-case name, returning 0 if not found.
        static const fld_traits* find_fld_traits(const std::string& lower_name);
        static const char FLD_NAME_END_CHAR; ///< field name end char (':')
        static const char EOL_CSTR[3]; ///< end-of-line ("\r\n")
    protected:
        hdr(int categories); ///< Construct.
        bool insert(int fld_traits_off, const std::string& v); ///< Insert.
        bool insert(const key& k, const std::string& value, bool update = false); ///< Insert.
        std::string find(int fld_traits_off) const; ///< Find.
        static void throw_bad_request(const char* name, const char* what); ///< Throw bad request.
        virtual void on_inserted(const_iterator it) {} ///< Inserted notification.
        const int categories_; ///< categories
    private:
        friend void RESTCGI_API copy(const env& e, request_hdr& rh, content_hdr& ch);
        map_type map_;
        static const fld_traits flds_traits_[]; ///< standard fields
        static const fld_traits* flds_traits_end_; ///< standard fields end
    };
    /** \brief General HTTP header fields.
     *
     * The cookies are parsed on requests automatically by this
     * header class. For responses the application must call
     * cookies() and insert any cookie to be sent in the response.
     * This will keep the "Set-Cookie" hdr field in sync with any
     * changes to cookies().
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.5 */
    class RESTCGI_API general_hdr : public hdr, private cookies::observer {
    public:
        typedef restcgi::cookies cookies_type; ///< cookies type
        std::string cache_control() const; ///< get cache control
        bool cache_control(const std::string& v); ///< set cache control
        std::string connection() const; ///< get connection
        bool connection(const std::string& v); ///< set connection
        bool date(date_time& v, const date_time& dflt) const; ///< get date as date_time
        bool date(const date_time& v); ///< set date from date_time
        std::string pragma() const; ///< get pragma
        bool pragma(const std::string& v); ///< set pragma
        std::string trailer() const; ///< get trailer
        bool trailer(const std::string& v); ///< set trailer
        std::string transfer_encoding() const; ///< get transfer encoding
        bool transfer_encoding(const std::string& v); ///< set transfer encoding
        std::string upgrade() const; ///< get upgrade
        bool upgrade(const std::string& v); ///< set upgrade
        std::string via() const; ///< get via
        bool via(const std::string& v); ///< set via
        std::string warning() const; ///< get warning
        bool warning(const std::string& v); ///< set warning
        cookies_type& cookies() {return cookies_;} ///< get cookies (see class description)
        const cookies_type& cookies() const {return cookies_;} ///< get cookies (see class description)
    protected:
        general_hdr(int categories); ///< Construct.
        ~general_hdr();
    private:
        void on_inserted(const_iterator it);
        void on_updated_reset();
        void on_updated(const cookies_type& v);
        bool on_updated_;
        cookies_type cookies_;
    };
    /** \brief Request and general HTTP header fields.
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.3 */
    class RESTCGI_API request_hdr : public general_hdr {
    public:
        request_hdr(); ///< construct
        std::string accept() const; ///< get accept
        std::string accept_charset() const; ///< get accept charset
        std::string accept_encoding() const; ///< get accept encoding
        std::string accept_language() const; ///< get accept language
        std::string authorization() const; ///< get authorization
        std::string expect() const; ///< get expect
        std::string from() const; ///< get from (URI??? mailto)
        std::string host() const; ///< get host (URI??? domain), ONLY VALID FOR HTTP 1.1 and above!!!
        std::string if_match() const; ///< get if match
        bool if_modified_since(date_time& v, const date_time& dflt) const; ///< get if modified since as date_time
        std::string if_none_match() const; ///< get if none match
        std::string if_range() const; ///< get if range
        bool if_unmodified_since(date_time& v, const date_time& dflt) const; ///< get if unmodified since as date_time
        bool max_forwards(size_t& v, size_t dflt) const; ///< get max forwards as size
        std::string proxy_authorization() const; ///< get proxy authorization
        std::string range() const; ///< get range
        std::string referer() const; ///< get referer (URI????)
        std::string te() const; ///< get te
        std::string user_agent() const; ///< get user agent
    };
    /** \brief Response, general, and some entity HTTP header fields.
     *
     * The entity fields that are related more to the entity itself
     * than its representation and are stored here (the others are
     * in the content_hdr). They are:<ul>
     * <li>allow</li>
     * <li>content_location</li>
     * <li>expires</li>
     * <li>last_modified</li>
     * </ul>
     * @see content_hdr
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.2
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec7.html#sec7.1 */
    class RESTCGI_API response_hdr : public general_hdr {
    public:
        response_hdr(); ///< construct
        std::string accept_ranges() const; ///< get accept ranges
        bool accept_ranges(const std::string& v); ///< set accept ranges
        bool age(size_t& v, size_t dflt) const; ///< get age as size
        bool age(size_t v); ///< set age from size
        std::string allow() const; ///< get allow
        bool allow(const std::string& v); ///< set allow
        std::string content_location() const; ///< get content location (URI????)
        bool content_location(const std::string& v); ///< set content location (URI????)
        std::string etag() const; ///< get etag
        bool etag(const std::string& v); ///< set etag
        bool expires(date_time& v, const date_time& dflt) const; ///< get expires as date_time
        bool expires(const date_time& v); ///< set expires from date_time
        bool last_modified(date_time& v, const date_time& dflt) const; ///< get last modified as date_time
        bool last_modified(const date_time& v); ///< set last modified from date_time
        std::string location() const; ///< get location (URI????)
        bool location(const std::string& v); ///< set location (URI????)
        std::string proxy_authenticate() const; ///< get proxy authenticate
        bool proxy_authenticate(const std::string& v); ///< set proxy authenticate
        bool retry_after(date_time& dt, size_t& secs) const; ///< get retry after
        bool retry_after(const date_time& v); ///< set retry after from date_time
        bool retry_after(size_t v); ///< set retry after from size
        std::string server() const; ///< get server
        bool server(const std::string& v); ///< set server
        std::string vary() const; ///< get vary
        bool vary(const std::string& v); ///< set vary
        std::string www_authenticate() const; ///< get www authenticate
        bool www_authenticate(const std::string& v); ///< set www authenticate
    };
    /** \brief Entity content HTTP header fields.
     *
     * These are the entity fields that
     * are directly related to the content in the response message, the
     * "representation" of the entity. The others are in the response_hdr.
     * @see response_hdr
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec7.html#sec7.1 */
    class RESTCGI_API content_hdr : public hdr {
    public:
        content_hdr(); ///< construct
        std::string content_encoding() const; ///< get content encoding
        bool content_encoding(const std::string& v); ///< set content encoding
        std::string content_language() const; ///< get content language
        bool content_language(const std::string& v); ///< set content language
        bool content_length(size_t& v, size_t dflt) const; ///< get content length as size
        bool content_length(size_t v); ///< set content length from size
        std::string content_md5() const; ///< get content md5
        bool content_md5(const std::string& v); ///< set content md5
        std::string content_range() const; ///< get content range
        bool content_range(const std::string& v); ///< set content range
        std::string content_type() const; ///< get content type
        bool content_type(const std::string& v); ///< set content type
    };
    /** \brief Stream out in HTTP header format. */
    inline std::ostream& operator <<(std::ostream& os, const hdr& v) {return v.operator <<(os);}
    /** \brief Copy the request and content headers from the environment.
     *
     * Note that the "general" and "other" fields are put into the request hdr, only
     * content-specific ones are put in the content hdr. */
    void RESTCGI_API copy(const env& e, request_hdr& rh, content_hdr& ch);
    /** \brief Creates a content hdr that has a field entry for the
     * content type given by the \c type arg. */
    content_hdr RESTCGI_API content_hdr_from_type(const std::string& type);
}
#endif
