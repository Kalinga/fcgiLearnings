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
#include "utils.h"
#include <uripp/utils.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#ifdef _WIN32
#define strtoll _strtoi64
#define strtoull _strtoui64
#endif
static const char iso_8859_1_nonctl_nonpercnt_chars_[] = { // graphic + space + soft-hyphen - percent
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    1,1,1,1,1,0,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, // 3f
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, // 7f
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
};
namespace restcgi {
    std::string RESTCGI_API encode_ctl(const std::string& v) {
        std::string::const_iterator f = v.begin();
        std::string::const_iterator anchor = f;
        std::string s;
        for (; f != v.end();)
            if (iso_8859_1_nonctl_nonpercnt_chars_[(unsigned char)*f])
                ++f;
            else {
                s.append(anchor, f);
                s.append(1, '%');
                uripp::append_hex(*f, s);
                anchor = ++f;
            }
        return (anchor == v.begin()) ? v : s.append(anchor, v.end());
    }
    bool tiny_parser::space(size_t size) {
        bool variable = !size;
        bool spaces = false;
        std::string::const_iterator f = first_;
        for (;; ++f) {
            if (f == last_ || !::isspace(*f)) {
                if (!variable)
                    return false;
                break;
            }
            spaces = true;
            if (!variable && !--size)
                break;
        }
        first_ = f;
        return spaces;
    }
    bool tiny_parser::operator ()(const char* p) {
        if (!p) return false;
        std::string::const_iterator f = first_;
        for (; *p; ++p, ++f) {
            if (f == last_)
                return false;
            if (*p != *f)
                return false;
        }
        first_ = f;
        return true;
    }
    int tiny_parser::digits(size_t num, bool leading0) {
        bool digfnd = false;
        int i = 0;
        for (; num; ++first_, --num) {
            if (first_ == last_)
                return -1;
            char c = *first_;
            if (::isdigit(c)) {
                digfnd = true;
                i = i * 10 + (c - '0');
            } else if (digfnd || leading0 || c != ' ')
                return -1;
        }
        return i;
    }
    int tiny_parser::array_item(const char** a, size_t size) {
        const char** end = a + size;
        for (const char** p = a; p < end; ++p)
            if (operator ()(*p))
                return p - a;
        return -1;
    }
}
