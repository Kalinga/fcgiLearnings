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
#ifndef restcgi_method_e_h
#define restcgi_method_e_h
#include "apidefs.h"
#include <string>
namespace restcgi {
    /** \brief Method type enumeration.
     *
     * Note that these are bit flags that can be used in a mask.
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html */
    class RESTCGI_API method_e {
    public:
        /// Enumeration.
        enum e {
            null, ///< none
            GET = 1 << 0, ///< get
            PUT = 1 << 1, ///< put
            POST = 1 << 2, ///< post
            DEL = 1 << 3, ///< delete (renamed to avoid macro and keyword conflicts)
            OPTIONS = 1 << 4, ///< options
            HEAD = 1 << 5, ///< head
            all = (GET | PUT | POST | DEL | OPTIONS | HEAD), ///< all
        };
        method_e() : e_(null) {} ///< Construct.
        method_e(const e& t) : e_(t) {} ///< Construct.
        method_e(const std::string& t); ///< Construct, set to null if not recognized.
        e enumeration() const {return e_;} ///< Get enumeration.
        bool is_null() const {return e_ == null;} ///< Test if null.
        bool operator ==(const method_e& rhs) const {return e_ == rhs.e_;} ///< Equal operator.
        bool operator !=(const method_e& rhs) const {return e_ != rhs.e_;} ///< Not equal operator.
        bool operator <(const method_e& rhs) const {return (int)e_ < (int)rhs.e_;} ///< Less operator.
        const char* cstring() const; ///< Get cstring.
        static std::string mask_to_string(int mask); ///< Get list of types in mask as comma-separated list.
    private:
        e e_;
        static const char* names_[];
    };
}
#endif
