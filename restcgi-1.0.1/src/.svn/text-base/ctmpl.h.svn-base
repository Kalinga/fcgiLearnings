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
#ifndef restcgi_ctmpl_h
#define restcgi_ctmpl_h
#include "apidefs.h"
#include "status_code_e.h"
#include <string>
#include <map>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    class env;
    /** \brief Template for creating content using variable replacement.
     *
     * This class stores the template string and content type
     * and has a method, eval(), for replacing the
     * escaped variable names, e.g. "%foo%", in the string
     * with the variables' corresponding values, if any. Variables
     * that are not defined are not replaced (resulting in the escaped
     * names remaining in the string).
     *
     * Note that the exception class has provisions for both
     * exception member data as variables and application-defined
     * variables. The templating mechanism is designed primarily for
     * the automatic creation of error response content from exceptions.
     * Some of the status codes, however, should not produce
     * any output content (see status_code_e documentation).
     *
     * Variable names include the chars: ASCII alpha upper and lower,
     * digits, underscore, hyphen. A name must begin with either
     * ASCII alpha or underscore. The escape char must immediately
     * precede and follow the name in the template.
     *
     * Variable names are looked up in the following places,
     * in order:
     * <ol><li>map: names defined by application, case-sensitive</li>
     * <li>env: HTTP header fields, request and content, names like "content-length", case-insensitive</li>
     * <li>env: other than HTTP header fields, names like SCRIPT_URL, case-sensitive</li></ol>
     *
     * Example:
     * \code
     * #include <restcgi/ctmpl.h>
     * using namespace restcgi;
     * ...
     * // method m is received for /cgi-bin/mycgi/myurl
     * ...
     * ctmpl ct("hello, %foo% from %SCRIPT_URL%!");
     * ctmpl::map_type map;
     * map.insert(std::make_pair("foo", "world"));
     * std::string out = ct.eval(m->env(), map);
     * // out == "hello, world from /cgi-bin/mycgi/myurl!"
     * \endcode
     * @see sc_ctmpls, method, exception */
    class RESTCGI_API ctmpl {
    public:
        typedef std::map<std::string, std::string> map_type; ///< map type
        /// Construct from template string and content type.
        ctmpl(const std::string& s, const std::string& content_type = "text/plain");
        /// Evaluate the constent template string in the context
        /// of the env and map (if any). See class description.
        std::string eval(const env& e, const map_type& map = map_type()) const;
        /// Evaluate the constent template string in the context
        /// of the map. See class description.
        std::string eval(const map_type& map) const;
        const std::string& content_type() const {return content_type_;} ///< Get content_type.
        /// Find variable value for name returning whether found.
        static bool find(const std::string& name, std::string& value, const env* e, const map_type& map);
        static const char ESCAPE_CHAR; ///< variable name escape char ('\%')
    private:
        std::string eval(const env* e, const map_type& map) const;
        std::string string_;
        std::string content_type_;
        static const char ctypes_[256];
    };
    /** \brief Map of status codes to content templates. */
    typedef std::map<status_code_e, ctmpl> sc_ctmpls;
}
#endif
