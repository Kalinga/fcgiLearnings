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
#ifndef restcgi_date_time_h
#define restcgi_date_time_h
#include "apidefs.h"
#include <time.h>
#include <string>
#include <iostream>
namespace restcgi {
    /** \brief Date and time class wrapping UTC time_t type for HTTP formats. */
    class RESTCGI_API date_time {
    public:
        typedef struct ::tm tm_type; ///< time structure type
        /// Construct null.
        date_time();
        /// Construct from time_t type (or null if t is 0).
        /// @exception std::invalid_argument if t negative
        date_time(const time_t& t);
        /// Construct from time structure. This checks to be sure that
        /// tm is in correct form. For example, the month day cannot
        /// be 43 and it cannot be 31 in Feb.
        /// @param tm time structure with time to construct
        /// @param check_wday if true the week day will be checked for correctness
        /// @exception std::invalid_argument if tm not valid
        date_time(const tm_type& tm, bool check_wday = false);
        /// Construct from given date. Note that y is 2004, not 104, and mon 9is 12 for Dec.
        date_time(unsigned short y, unsigned char mon, unsigned char d, unsigned char h, unsigned char min, unsigned char s);
        /// Construct from string. No isspace chars allowed before or after.
        /// Acceptable formats:
        /// <pre>
        /// Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
        /// Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
        /// Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
        /// </pre>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.3.1
        /// @exception std::invalid_argument on conversion error
        date_time(const std::string& s);
        bool is_null() const {return time_ == NULL_TIME;} ///< Test if null.
        bool operator ==(const date_time& rhs) const {return time_ == rhs.time_;} ///< Equal operator.
        bool operator !=(const date_time& rhs) const {return time_ != rhs.time_;} ///< Not equal operator.
        bool operator <(const date_time& rhs) const; ///< Less operator (null is bottom).
        operator time_t() const {return time_;} ///< Get time_t.
        /// Add seconds and return result.
        /// @exception std::invalid_argument on over/underflow
        date_time operator +(int seconds) const {
            date_time result = *this;
            return result += seconds;
        }
        /// Add seconds to this and return this.
        /// @exception std::invalid_argument on over/underflow
        date_time& operator +=(int seconds);
        /// Subtract seconds and return result.
        /// @exception std::invalid_argument on over/underflow
        date_time operator -(int seconds) const {
            date_time result = *this;
            return result -= seconds;
        }
        /// Subtract seconds to this and return this.
        /// @exception std::invalid_argument on over/underflow
        date_time& operator -=(int seconds);
        /// Convert to time structure (not cached).
        /// @exception std::domain_error if null
        tm_type tm() const;
        /// Convert to HTTP format string (not cached).
        /// Format (RFC 822, updated by RFC 1123):
        /// <pre>
        /// Sun, 06 Nov 1994 08:49:37 GMT
        /// </pre>
        /// @exception std::domain_error if null
        std::string string() const;
        /// Convert to ISO 8601 format string (not cached).
        /// <pre>
        /// 1994-11-06T08:49:37Z
        /// </pre>
        /// @exception std::domain_error if null
        std::string iso_string() const;
        static date_time now(); ///< Get current time.
        /// Assert struct tm is within range. This does not to a complete
        /// validity check as it misses Feb 31 errors.
        /// @param v time structure with time to test
        /// @param check_wday if true the week day will be checked for correctness
        /// @exception std::invalid_argument if not valid
        static void assert_in_range(const tm_type& v, bool check_wday);
        /// Convert to GMT time_t.
        /// @param tm time structure with time to convert
        /// @param check_wday if true the week day will be checked for correctness
        /// @exception std::invalid_argument if tm is not valid
        static time_t to_time(const tm_type& tm, bool check_wday);
        /// Convert to GMT time structure.
        /// @exception std::invalid_argument if t is null (0)
        static tm_type to_tm(const time_t& t);
        /// Convert to GMT time structure.
        /// @exception std::invalid_argument if t is null (0)
        static void to_tm(const time_t& t, tm_type& tm);
        static const time_t NULL_TIME; ///< null (0)
    private:
        time_t time_;
    };
    /** \brief Parse a date_time, returning whether found or not
     * and advancing first and setting v if found.
     *
     * Skips leading isspace chars.
     * @exception std::invalid_argument if a complete pattern is found but the date or time is invalid */
    bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, date_time& v);
}
/** \brief URI library namespace additions.
 *
 * These functions are added to uripp so that
 * there is a single namespace for converting both URI- and HTTP-related data types. */
namespace uripp {
	std::string RESTCGI_API convert(const restcgi::date_time& v); ///< date and time to string
	/** \brief String to date and time, returning true if set, which is
	 * when the string is not empty or all isspace.
	 *
	 * Space before and/or after value OK.
	 * @exception std::invalid_argument on conversion error */
	bool RESTCGI_API convert(const std::string& s, restcgi::date_time& v);
}
#endif
