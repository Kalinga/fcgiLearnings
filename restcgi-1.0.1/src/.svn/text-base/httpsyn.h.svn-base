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
#ifndef restcgi_httpsyn_h
#define restcgi_httpsyn_h
#include "apidefs.h"
#include <string>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    /** \brief HTTP/1.1 specification "token".
     *
     * Notes:<ol>
     * <li>ISO 8859-1 charset not completely valid, only US-ASCII.</li>
     * <li>Meant to be human readable, i.e. CTL chars not valid.</li>
     * <li>Printable chars that are used in the HTTP header syntax,
     *     e.g. "=", are not allowed (see Syntax below).</li>
     * <li>There is no "encoding"/"decoding" of the token string
     *     (encoding() method included for consistency).
     *     The concept simply restricts the chars allowed in the string.
     * <li>The percent sign is valid so invalid chars could be encoded
     *     using URI percent-encoding, but en/decoding is left to the
     *     caller (must be consistent between all apps!).</li>
     * </ol>
     *
     * Syntax:
     * <pre>
     * CHAR       = \<any US-ASCII character (octets 0 - 127)>
     * CTL        = \<any US-ASCII control character (octets 0 - 31)
     *              and DEL (127)>
     * SP         = \<US-ASCII SP, space (32)>
     * HT         = \<US-ASCII HT, horizontal-tab (9)>
     * token      = 1*\<any CHAR except CTLs or separators>
     * separators = "(" | ")" | "<" | ">" | "@"
     *            | "," | ";" | ":" | "\" | \<">
     *            | "/" | "[" | "]" | "?" | "="
     *            | "{" | "}" | SP | HT
     * </pre>
     * @see http_word, parse(std::string::const_iterator&, std::string::const_iterator, http_token&)
     * @see urisyn::encode, urisyn::decode
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec2.html#sec2.2 */
    class RESTCGI_API http_token {
    public:
        http_token(); ///< Construct.
        /// Construct from string.
        /// @exception std::invalid_argument if string contains invalid chars or is empty
        http_token(const std::string& s);
        bool is_null() const {return string_.empty();} ///< Test if null.
        const std::string& string() const {return string_;} ///< Get string.
        const std::string& encoding() const {return string_;} ///< Encoding, just returns string.
        static bool is_valid(const std::string& v);///< Test if valid http_token.
        static const char ctypes_[256]; ///< valid chars
    private:
        friend bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, http_token& v, bool noskipls);
        std::string string_;
    };
    /** \brief Parse HTTP token.
     *
     * Returns whether found or not
     * and advances first and sets v if found.
     * Skips leading SP or HT chars unless \p noskipls is true. */
    bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, http_token& v, bool noskipls = false);
    /** \brief HTTP/1.1 specification "TEXT".
     *
     * Notes:<ol>
     * <li>Meant to be human readable, i.e. CTL chars not valid.</li>
     * <li>There is no "encoding"/"decoding" of the TEXT string
     *     (encoding() method included for consistency).
     *     The concept simply restricts the chars allowed in the string.</li>
     * <li>On construction, LWS is compressed to a single space char.
     *     Note that this is irreversible, encoding does not put
     *     the LWS back in.</li>
     * </ol>
     *
     * Syntax (see http_token for additional definitions):
     * <pre>
     * OCTET         = \<any 8-bit sequence of data>
     * TEXT          = \<any OCTET except CTLs, but including LWS>
     * LWS           = [CRLF] 1*( SP | HT )
     * CRLF          = CR LF
     * CR            = \<US-ASCII CR, carriage return (13)>
     * LF            = \<US-ASCII LF, linefeed (10)>
     * </pre>
     * @see http_token, http_word
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec2.html#sec22 */
    class RESTCGI_API http_text {
    public:
        http_text(); ///< Construct.
        /// Construct from string.
        /// @exception std::invalid_argument if string contains invalid chars
        http_text(const std::string& s);
        bool empty() const {return string_.empty();} ///< Test empty.
        const std::string& string() const {return string_;} ///< Get string.
        const std::string& encoding() const {return string_;} ///< Encoding, just returns string.
        /// Append (and check) string and return result.
        /// @exception std::invalid_argument if string contains invalid chars
        http_text operator +(const std::string& rhs) const {
            http_text result = *this;
            return result += rhs;
        }
        /// Append (and check) string to this and return this.
        /// @exception std::invalid_argument if string contains invalid chars
        http_text& operator +=(const std::string& rhs);
        /// Append text and return result.
        http_text operator +(const http_text& rhs) const {
            http_text result = *this;
            return result += rhs;
        }
        /// Append text to this and return this.
        http_text& operator +=(const http_text& rhs) {
            string_.append(rhs.string_);
            return *this;
        }
        static const char ctypes_[256]; ///< valid chars (1=non space, 2=space)
    private:
        friend bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, http_text& v, const char* terms, bool lastok);
        std::string string_;
    };
    /** \brief Parse HTTP text.
     *
     * Returns whether found or not
     * and advances first and sets v if found.
     * LWS is compressed. If terms is specified it stops
     * on any of those chars, i.e. on return <code>*first == \<term></code>
     * (note that true is returned and v is empty if term is the first
     * char). If last encountered before a term char and lastok is true,
     * this returns all chars in interval. If terms not specified,
     * this returns all chars in interval or up to first invalid char. */
    bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, http_text& v, const char* terms = 0, bool lastok = false);
    /** \brief HTTP/1.1 specification "word", either a "token" or
     * quoted text.
     *
     * This is meant for converting strings with
     * spaces and special chars, e.g. ';', into a single "word" for
     * HTTP "value" type fields. When encoding words, if the string
     * parses as a "token" it is returned as is, otherwise it is
     * quoted, escaping any embedded '\\' or '"' chars.
     *
     * Note: An alternative to this is to perform a "generic" URI
     * percent-encoding along the lines of the JavaScript encode() function,
     * producing a "token" instead of a "quoted-string". However, the
     * JavaScript functions will allow (i.e. not encode) some chars that
     * are invalid as "token" chars. Depending on the context these
     * differences may or may not cause problems.<ul>
     * <li>JavaScript encode(): allows fewer chars than "token" but conflicts
     *     by allowing forward slash and at symbol. This is frequently
     *     used to set cookie values from within the browser.</li>
     * <li>JavaScript encodeURIComponent(): allows fewer chars than "token"
     *     but conflicts by allowing left and right parens.</li>
     * </ul>
     *
     * There is no standard solution to the encoding problem. Applications
     * that share data must be explicitly programmed to do so. In other words,
     * using this class for both encoding and decoding will work,
     * interoperability with other applications (without explicit, additional
     * programming) is not guaranteed.
     *
     * Syntax (see http_token, http_text for additional definitions):
     * <pre>
     * word          = token | quoted-string
     * quoted-string = ( \<"> *(qdtext | quoted-pair ) \<"> )
     * qdtext        = \<any TEXT except \<">>
     * quoted-pair   = "\" CHAR
     * </pre>
     *
     * Notes on quoted-string:<ul>
     * <li>'\' is used as an escape for any char (0-0x7F) that
     *     follows it, and, thus, the '\' will be removed.</li>
     * <li>CTL chars (including HT) are not allowed, escaped or not,
     *     and will cause a return of false if encountered before
     *     the ending quote.</li>
     * <li>The char after the ending quote is not checked in any way.</li>
     * <li>Chars from other charsets not supported, i.e. RFC2047 is
     *     not supported.</li>
     * </ul>
     * @see http_token, http_text, parse(std::string::const_iterator&, std::string::const_iterator, http_word&)
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec2.html#sec2.2 */
    class RESTCGI_API http_word {
    public:
        http_word(); ///< Construct.
        /// Construct from encoded string, either http_token or quoted http_text.
        /// @exception std::invalid_argument if string not valid word
        http_word(const std::string& s);
        http_word(const http_token& tok); ///< Construct from HTTP token.
        http_word(const http_text& txt); ///< Construct from HTTP text.
        bool empty() const {return string_.empty();} ///< Test empty.
        /// Get (unencoded) string. This is either the token or the
        /// unquoted (unfolded) text.
        const std::string& string() const {return string_;}
        /// Append (and check) text string and return result.
        /// @exception std::invalid_argument if string contains invalid text chars
        http_word operator +(const std::string& rhs) const {
            http_word result = *this;
            return result += rhs;
        }
        /// Append (and check) text string to this and return this.
        /// @exception std::invalid_argument if string contains invalid text chars
        http_word& operator +=(const std::string& rhs);
        /// Append text and return result.
        http_word operator +(const http_text& rhs) const {
            http_word result = *this;
            return result += rhs;
        }
        http_word& operator +=(const http_text& rhs); ///< Append text to this and return this.
        /// Append word and return result.
        http_word operator +(const http_word& rhs) const {
            http_word result = *this;
            return result += rhs;
        }
        http_word& operator +=(const http_word& rhs); ///< Append word to this and return this.
        /// Calculate the encoding for the word, returning just the
        /// string if a token, otherwise quoting and escaping.
        std::string encoding(bool force_quotes = false) const;
        static const char ESC_CHAR; ///< escape char ('\\')
    private:
        friend bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, http_word& v);
        std::string string_;
        bool token_;
    };
    /** \brief Parse HTTP word.
     *
     * Returns whether found or not and advances first and sets if found.
     * Skips leading SP or HT chars. */
    bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, http_word& v);
    /** \brief HTTP syntax declarations.
     *
     * These parse functions work on HTTP char class definitions. */
    namespace httpsyn {
        /// Test if HTTP specification space char, SP or HT.
        inline bool isspace(char c) {return c == ' ' || c == '\t';}
        /// Parse literal, returning whether found or not and advancing
        /// first (past literal) if found. Skips leading SP or HT chars.
        bool RESTCGI_API parse(std::string::const_iterator& first, std::string::const_iterator last, const char* v);
        /// Parse looking for any non-space char, returning whether found or
        /// not and advancing first to non-space if found.
        bool RESTCGI_API parse_any(std::string::const_iterator& first, std::string::const_iterator last);
        /// Parse end-of-range, returning whether found or not
        /// and advancing first (to last) if found. Skips
        /// SP or HT chars.
        bool RESTCGI_API parse_eor(std::string::const_iterator& first, std::string::const_iterator last);
        /// Parse looking for term chars skipping all other chars
        /// and advancing first (past term char) if found.
        bool RESTCGI_API parse_skip(std::string::const_iterator& first, std::string::const_iterator last, const char* terms);
    }
}
#endif

