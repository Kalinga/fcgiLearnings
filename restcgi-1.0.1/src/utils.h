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
#ifndef restcgi_utils_h
#define restcgi_utils_h
#include "apidefs.h"
#include <string>
#include <stack>
namespace restcgi {
    /** \brief URI encode all (ISO 8859) control chars.
     *
     * This is useful as a way
     * of making CTL chars printable, for error msgs, for example. */
    std::string RESTCGI_API encode_ctl(const std::string& v);
    /** \brief Tiny parser. */
    class tiny_parser {
    public:
        /// Construct.
        tiny_parser(std::string::const_iterator first, std::string::const_iterator last): first_(first), last_(last) {}
        bool space(size_t size = 0); ///< match isspace chars, variable size if size 0
        bool operator ()(const char* p); ///< match literal
        int digits(size_t num, bool leading0 = true); ///< match digits, -1 means no
        int array_item(const char** a, size_t size); ///< match array item, -1 means no
        bool at_end() const {return first_ == last_;} ///< test at end
        void push() {stack_.push(first_);} ///< save first
        void pop() {first_ = stack_.top(); stack_.pop();} ///< restore first
        const std::string::const_iterator& first() const {return first_;} ///< first (next position)
    private:
        std::string::const_iterator first_;
        std::string::const_iterator last_;
        std::stack<std::string::const_iterator> stack_;
    };
}
#endif
