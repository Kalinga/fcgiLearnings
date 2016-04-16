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
#ifndef restcgi_env_h
#define restcgi_env_h
#include "apidefs.h"
#include <string>
#include <map>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    /** \brief CGI environment variables.
     *
     * This pulls the CGI information out of the process environment,
     * e.g. SERVER_PORT. Note that some of this information is stored
     * in the other library classes where appropriate, in particular,
     * the hdr classes.
     *
     * Example: GET http://mywebserver.com/cgi-bin/mycgi.exe/hello/foo?abc=a%20b%20c
     * <pre>
     * REQUEST_METHOD=GET
     * SCRIPT_URI=http://mywebserver.com/cgi-bin/mycgi.exe/hello/foo
     * REQUEST_URI=/cgi-bin/mycgi.exe/hello/foo?abc=a%20b%20c
     * SCRIPT_URL=/cgi-bin/mycgi.exe/hello/foo
     * SCRIPT_NAME=/cgi-bin/mycgi.exe
     * PATH_INFO=/hello/foo
     * QUERY_STRING=abc=a%20b%20c
     * </pre>
     *
     * fcgid_module differences (WINDOWS only):
     * <pre>
     * SCRIPT_NAME=/cgi-bin
     * PATH_INFO=/mycgi.exe/hello/foo
     * </pre>
     *
     * Attribution to NCSA Software Development Group, cgi@ncsa.uiuc.edu.
     * @see http://hoohoo.ncsa.uiuc.edu/cgi/env.html
     * @see hdr */
    class RESTCGI_API env {
    public:
        typedef std::map<std::string, std::string> map_type; ///< map type
        /// Construct.
        env();
        /// Construct from null-terminated array of strings (for testing).
        env(const char **pcstr);
        /// Construct from map (for testing).
        env(const map_type& m);
        /// The name and version of the information server software
        /// answering the request (and running the gateway).
        /// Format: name/version
        std::string server_software() const;
        /// The server's hostname, DNS alias, or IP address as it
        /// would appear in self-referencing URLs.
        std::string server_name() const;
        /// The revision of the CGI specification to which this server
        /// complies. Format: CGI/revision
        std::string gateway_interface() const;
        /// The name and revision of the information protcol this request
        /// came in with. Format: protocol/revision
        std::string server_protocol() const;
        /// The port number to which the request was sent.
        std::string server_port() const;
        /// The method with which the request was made. For HTTP,
        /// this is "GET", "HEAD", "POST", etc.
        std::string request_method() const;
        /// The path information without the script_name()
        /// as given by the client. In other words, scripts can be
        /// accessed by their virtual pathname, followed by extra information
        /// at the end of this path. The extra information is sent as
        /// PATH_INFO. This information is decoded by the server
        /// if it comes from a URL before it is passed to the CGI script.
        /// @see script_name()
        std::string path_info() const;
        /// The server provides a translated version of PATH_INFO, which
        /// takes the path and does any virtual-to-physical mapping to it.
        /// This is where the document file would have been if the
        /// script_name() had not been in the URL (not very useful).
        std::string path_translated() const;
        /// A virtual path to the script being executed, used for
        /// self-referencing URLs. This is the original path before any
        /// rewrites, i.e. if rewriting inserts a script name into the path
        /// it is not included here. When prefixed to the path_info() gives
        /// the script_url().
        /// @see path_info()
        std::string script_name() const;
        /// This is the original script_name() plus path_info() plus
        /// query_string().
        /// @see script_name()
        std::string request_uri() const;
        /// File system path to the script being executed.
        /// Note that this is independent of rewrites.
        std::string script_filename() const;
        /// The original path, which is path_info() with the original
        /// cgi-script name prefixed to it.
        /// This is the request_uri() minus the query_string().
        /// @see http://httpd.apache.org/docs/2.2/mod/mod_rewrite.html#EnvVar
        std::string script_url() const;
        /// The original URL minus query_string(). In other words, this is
        /// script_url() with "http://" and hostname prefixed to it.
        /// @see http://httpd.apache.org/docs/2.2/mod/mod_rewrite.html#EnvVar
        std::string script_uri() const;
        /// The information which follows the ? in the URL which referenced
        /// this script. This is the query information. It should not be
        /// decoded in any fashion. This variable should always be set when
        /// there is query information, regardless of command line decoding.
        std::string query_string() const;
        /// The hostname making the request. If the server does not have
        /// this information, it should set REMOTE_ADDR and leave this unset.
        std::string remote_host() const;
        /// The IP address of the remote host making the request.
        std::string remote_addr() const;
        /// If the server supports user authentication, and the script is
        /// protects, this is the protocol-specific authentication method
        /// used to validate the user.
        std::string auth_type() const;
        /// If the server supports user authentication, and the script is
        /// protected, this is the username they have authenticated as.
        std::string remote_user() const;
        /// If the HTTP server supports RFC 931 identification, then this
        /// variable will be set to the remote user name retrieved from
        /// the server. Usage of this variable should be limited to
        /// logging only.
        std::string remote_ident() const;
        /// Error script: This is the request as sent exactly to the server.
        std::string redirect_request() const;
        /// Error script: This is the requested URL that caused the error.
        std::string redirect_url() const;
        /// Error script: This is the status number and message that would
        /// have been sent if it would have been allowed to reply.
        std::string redirect_status() const;
        /// For queries which have attached information, such as HTTP POST
        /// and PUT, this is the content type of the data.
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.17
        std::string content_type() const;
        /// The length of the said content as given by the client.
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.13
        std::string content_length() const;
        /// The MIME types which the client will accept, as given by HTTP
        /// headers. Other protocols may need to get this information from
        /// elsewhere. Each item in this list should be separated by commas
        /// as per the HTTP spec. Format: type/subtype, type/subtype
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.1
        std::string accept() const;
        /// Charsets acceptable for response.
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.2
        std::string accept_charset() const;
        /// Restricts the content-codings that are acceptable in the response.
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.3
        std::string accept_encoding() const;
        /// Restricts the set of natural languages that are preferred as a
        /// response to the request.
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.4
        std::string accept_language() const;
        /// The browser the client is using to send the request.
        /// General format: software/version library/version.
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.43
        std::string user_agent() const;
        /// Find HTTP header field value, returning value and true if found.
        /// This uses to_env_name() to get the field value from the process env.
        bool hdr_find(const std::string& hname, std::string& value) const;
        /// Find environment variable value, returning value and true if found.
        bool find(const char* name, std::string& value) const;
        /// Find environment variable value, returning value or empty string
        /// if not found.
        std::string find(const char* name) const;
        /** \brief Iterator over the header field string pairs: name, value. */
        class RESTCGI_API hdr_iterator {
        public:
            typedef std::pair<std::string, std::string> referent_type; ///< referent type
            hdr_iterator& operator ++(); ///< pre-increment
            hdr_iterator operator ++(int); ///< post-increment
            bool operator ==(const hdr_iterator& rhs) const; ///< equals operator
            bool operator !=(const hdr_iterator& rhs) const {return !operator ==(rhs);} ///< not equals operator
            const referent_type& operator *() const; ///< dereference
            const referent_type* operator ->() const; ///< reference
        private:
            friend class env;
            hdr_iterator() : end_(true) {}
            hdr_iterator(const env& e);
            void increment(bool initialize = false);
            bool end_;
            char** p_;
            map_type::const_iterator it_;
            map_type::const_iterator it_end_;
            referent_type current_;
        };
        hdr_iterator hdr_begin() const {return hdr_iterator(*this);} ///< Header fields beginning.
        hdr_iterator hdr_end() const {return hdr_iterator();} ///< Header fields end.
        /// Override name with value. Useful for "preprocessing" of REST input.
        void override(const std::string& name, const std::string& value);
        /// Uppercase the name, translate "-" to "_" and prefix "HTTP_". This
        /// is the format used to store the value in the process environment.
        /// For example, the name "content-length" would
        /// be translated to HTTP_CONTENT_LENGTH.
        static std::string to_env_name(const std::string& hname);
        /// Inverse of to_env_name() returning all lower case and "-"s.
        static std::string from_env_name(const std::string& ename);
    private:
        friend class hdr_iterator;
        bool map_override_mode_;
        map_type map_;
    };
}
#endif
