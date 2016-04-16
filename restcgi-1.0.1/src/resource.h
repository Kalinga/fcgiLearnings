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
#ifndef restcgi_resource_h
#define restcgi_resource_h
#include "apidefs.h"
#include "method_e.h"
#include "content.h"
#include "status_code_e.h"
#include "version.h"
#include <uripp/path.h>
#include <uripp/query.h>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
namespace restcgi {
    class method;
    class version;
    class version_tag;
    class request_hdr;
    class response_hdr;
    class content_hdr;
    /** \brief URI information for the located resource. */
    class RESTCGI_API uri_info {
    public:
        typedef restcgi::method method_type; ///< method type
        typedef boost::shared_ptr<method_type> method_pointer; ///< method shared ptr
        uri_info(); ///< Construct.
        uri_info(method_pointer m); ///< Construct.
        const uripp::path& path() const; ///< Get path from CGI (path used in resource::locate()).
        const uripp::query& query() const; ///< Get query.
        void method(method_pointer m); ///< Set method.
    private:
        method_pointer method_;
    };
    /** \brief Base class for REST resource.
     *
     * The application uses this
     * as the base class for its resources or as the base class
     * for proxies for its resources (non-intrusive).
     *
     * The basic operation of resources is as follows:<ol>
     * <li>locate() is called recursively on this class but
     *     on different resource objects to narrow to the
     *     resource identified in the method. The application
     *     is creating the resource objects as this proceeds.</li>
     * <li>On the located resource object, this class's method
     *     is called which corresponds with the type in the
     *     request: get(), put(), etc.</li>
     * <li>This class implements the method, e.g. get(), as
     *     series of calls to the derived class, e.g. read().
     *     The method itself can be overridden, but it is usually
     *     not necessary. The submethods must be overridden by the
     *     derived class to do the actual work.</li>
     * <li>Exceptions can be thrown and will be automatically
     *     processed. The explicit status exceptions, e.g.
     *     bad_request, will result in the given status returned
     *     in the response.</li>
     * </ol>
     *
     * Note that the web server handles the TRACE method without
     * calling the application.
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
     * @see rest */
    class RESTCGI_API resource {
    public:
        typedef boost::shared_ptr<resource> pointer; ///< shared ptr
        typedef boost::shared_ptr<const resource> const_pointer; ///< const shared ptr
        typedef restcgi::method method_type; ///< method type
        typedef boost::shared_ptr<method_type> method_pointer; ///< method shared ptr
        typedef uripp::path uri_path_type; ///< URI path type
        typedef restcgi::uri_info uri_info_type; ///< uri_info type
        typedef restcgi::version version_type; ///< version type
        typedef restcgi::version_tag version_tag_type; ///< version tag type
        typedef restcgi::version_constraint version_constraint_type; ///< version_constraint type
        virtual ~resource();
        void method(method_pointer m); ///< Set method (called before locate() by rest processing).
        method_pointer method() const {return method_;} ///< Get method (valid before and after locate()).
        /// Locate the resource at the given path relative to this
        /// and update path. This method provides the main means
        /// for navigating to the resource specified by the client in
        /// terms of the URI path. The application should organize its
        /// resources in a hierarchy (basic REST architecture) which
        /// can be descended by repeated calls to this method on the
        /// returned resources and the updated path (typically
        /// using pop_front() at each step).
        ///
        /// Result possibilities:
        /// <ol><li>a null pointer is returned and the path is empty
        /// then method is performed on this,</li>
        /// <li>null pointer is returned and path is not empty
        /// then a bad_request exception is automatically thrown,</li>
        /// <li>pointer to another resource returned then that
        /// resource's locate() method is called if the (potentially)
        /// updated path is not empty otherwise the method is
        /// performed on that resource.</li></ol>
        ///
        /// The default is to perform the method on this, but will
        /// result in bad_request if path is not empty.
        /// @exception exception restcgi exceptions are rethrown, others
        /// wrapped and rethrown as a bad_request
        virtual pointer locate(uri_path_type& path);
        /// Respond with a representation of this resource in the
        /// output content.
        ///
        /// Unless overridden (usually not necessary), this will call the
        /// following methods:<ol>
        /// <li>read(): Read resource from application store (required).</li>
        /// <li>If version returned, assert constraint (if any)
        ///     satisfied.</li>
        /// <li>Initialize status code (OK) and response hdr (with version
        ///     info).</li>
        /// <li>on_responding(): Modify status, response and content hdrs
        ///     (optional). (Not called if default_response set.)</li>
        /// <li>write(): Write to content stream (optional but customary).
        ///     (Not called if default_response set.)</li>
        /// </ol>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html#sec9.3
        virtual void get_method();
        /// Update the resource with the input content and, optionally,
        /// send a representation of the updated resource in the
        /// output content. The resource must already exist and this is
        /// designed to change one or more of its attributes.
        ///
        /// Unless overridden (usually not necessary), this will call the
        /// following methods:<ol>
        /// <li>read(): Read resource from application store (required).</li>
        /// <li>If version returned, assert constraint (if any) satisfied.</li>
        /// <li>update(): Update the resource from the input content and
        ///     save to application store (required).</li>
        /// <li>Initialize status code (OK) and response hdr (with version
        ///     info).</li>
        /// <li>on_responding(): Modify status, response and content hdrs
        ///     (optional). (Not called if default_response set.)</li>
        /// <li>write(): Write to content stream (optional).
        ///     (Not called if default_response set.)</li>
        /// </ol>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html#sec9.6
        virtual void put_method();
        /// Create a subordinate (child) resource with the input content
        /// and, optionally, send a representation of the child
        /// resource in the output content. Note that the URI path points
        /// to the containing (parent) resource which is this resource.
        ///
        /// Unless overridden (usually not necessary), this will call the
        /// following methods:<ol>
        /// <li>create_child(): Create child resource (required).</li>
        /// <li>On child resource:<ol>
        ///     <li>create(): Create the resource from the input content and
        ///         save to application store (required).</li>
        ///     <li>Initialize status code (CREATED) and response hdr
        ///         (with version info and content-location to this uri path
        ///         appended with the returned id).</li>
        ///     <li>on_responding(): Modify status, response and content hdrs
        ///         (optional). (Not called if child default_response set.)</li>
        ///     <li>write(): Write to content stream (optional).
        ///         (Not called if child default_response set.)</li>
        ///     </ol></li>
        /// </ol>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html#sec9.5
        virtual void post_method();
        /// Delete this resource.
        ///
        /// Unless overridden (usually not necessary), this will call the
        /// following methods:<ol>
        /// <li>If request contained a version constraint:<ol>
        ///     <li>read(veronly=true): Read resource version info from
        ///         application store (required).</li>
        ///     <li>If version returned, assert constraint (if any)
        ///         satisfied.</li>
        ///     </ol></li>
        /// <li>del(): Delete the resource from the application store
        ///     (required).</li>
        /// <li>Initialize status code (OK).</li>
        /// <li>on_responding(): Modify status, response hdr (optional).
        ///     (Not called if default_response set.)</li>
        /// <li>write(): Write to content stream (optional).
        ///     (Not called if default_response set.)</li>
        /// </ol>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html#sec9.7
        virtual void del_method();
        /// Respond with the "options" available for the resource and,
        /// optionally, send a representation of the "options" in the
        /// output content.
        ///
        /// The required, minimum response, and the default behavior of this
        /// base class, is to return the Allow header field based on
        /// the "methods allowed mask" set at construction and not send
        /// content (note that Content-Length must be explicitly set to 0).
        /// See RFC. The special case of the URI path "*", essentially a
        /// "ping", is handled elsewhere and does not result in a call
        /// to any of the resource objects.
        ///
        /// Unless overridden (usually not necessary), this will call the
        /// following methods:<ol>
        /// <li>Initialize status code (OK) and response hdr (allow field
        ///     is set to methods_allowed_mask).</li>
        /// <li>on_responding(): Modify status, response and content hdrs
        ///     (optional).</li>
        /// </ol>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html#sec9.2
        virtual void options_method();
        /// Respond with header fields the same as if this were a get()
        /// but do not return output content. The application may be able
        /// to optimize the read for the HEAD case.
        ///
        /// Unless overridden (usually not necessary), this will call the
        /// following methods:<ol>
        /// <li>read(): Read resource from application store (required).</li>
        /// <li>If version returned, assert constraint (if any)
        ///     satisfied.</li>
        /// <li>Initialize status code (OK) and response hdr (with version
        ///     info).</li>
        /// <li>on_responding(): Modify status, response and content hdrs
        ///     (optional).</li>
        /// </ol>
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html#sec9.4
        virtual void head_method();
        int methods_allowed_mask() const {return methods_allowed_mask_;} ///< Get methods-allowed mask.
        pointer child() const {return child_;} ///< Get child pointer (MUST set chain_children in root).
        pointer parent() const {return parent_.lock();} ///< Get parent pointer (MUST set chain_children in root).
        bool chain_children() const {return chain_children_;} ///< Chain children?
        void child(pointer rthis, pointer cr); ///< Set child.
    protected:
        typedef uripp::query uri_query_type; ///< uri_query type
        /// Construct. The methods that this resource supports
        /// must be declared by or'ing the corresponding method_e enums
        /// into the mask argument, e.g. method_e::GET | method_e::DEL.
        /// This base class has default implementations for OPTIONS
        /// and HEAD (if GET is implemented), but they must be put into
        /// the given mask by the derived class in order for this class
        /// to use them. If chain_children is set in root (MUST be root)
        /// all resources located along URI path are linked. This will
        /// enable the child() and parent() methods.
        ///
        /// Application programming note: THE MASK MUST BE KEPT IN SYNC WITH
        /// METHOD IMPLEMENTATIONS!!!
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html#sec10.4.6
        /// @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.7
        resource(int methods_allowed_mask, bool chain_children = false);
        const method_e& me() const; ///< Get method enum, for example method_e::GET (valid after locate()).
        const uri_info_type& uri_info() const {return uri_info_;} ///< Get URI info (valid after locate()).
        /// Get version constraint (valid after locate()).
        version_constraint_type version_constraint() const {return version_constraint_;}
        /// Set default response. Seting to true causes on_responding() and
        /// write() methods to be skipped. This is convenient for resources
        /// that handle multiple methods when some do not require a custom
        /// response. (See above.)
        void default_response(bool v);
        /// Read the attributes of the resource from the application
        /// store and return the current version information. This is
        /// called for get(), put(), del(), and head(). But, note that
        /// for del() this is only called if there is a version
        /// constraint and then \p veronly is set to true to indicate that
        /// all the attributes are not needed. For head() the application
        /// may be able to optimize the resource read. If versioning is
        /// not being used a null version can be returned and constraints
        /// will be ignored.
        ///
        /// Default is to throw internal_server_error.
        /// @exception exception application can throw any exception
        virtual version_type read(bool veronly);
        /// Update the attributes of the resource and save to the
        /// application store, returning the new version information.
        /// This is called for put(). If versioning is not being used
        /// a null version can be returned.
        ///
        /// Default is to throw internal_server_error.
        /// @exception exception application can throw any exception
        virtual version_type update(icontent::pointer ic);
        /// Create a child resource. This is called for post().
        ///
        /// Default is to throw internal_server_error.
        /// @exception exception application can throw any exception
        virtual pointer create_child();
        typedef std::pair<uri_path_type, version_type> id_version_type; ///< id version pair type
        /// Create the resource from the input content and save to the
        /// application store, returning the id and version information.
        /// This is called on the child resource for a POST on th parent. The id
        /// is a single-segment URI path and is used to set the content-location
        /// response hdr field (the parent path plus this child id).
        /// If versioning is not being used a null version can be returned.
        ///
        /// Default is to throw internal_server_error.
        /// @exception exception application can throw any exception
        virtual id_version_type create(icontent::pointer ic);
        /// Delete the resource from the application store.
        ///
        /// Default is to throw internal_server_error.
        /// @exception exception application can throw any exception
        virtual void del();
        /** Modify status code and response and content hdrs prior to
         * responding. This class sends the response but calls this
         * before doing it so that the derived class can add header fields,
         * e.g. content-type. The version information, if any has already
         * been added to the hdrs. Note methods that do not write output
         * content, e.g. del(), will ignore any changes to the content_hdr.
         * In other words, the content-type can always be set regardless of
         * method type.
         *
         * "hello, world" example (see also write()). This is all that is
         * needed here:
         * \code
         * ch.content_type("text/plain");
         * \endcode
         *
         * If needed, the method type can be checked as follows:
         * \code
         * if (me() == method_e::GET) ...
         * \endcode
         *
         * Default is to do nothing (use status code and hdrs as they are). */
        virtual void on_responding(status_code_e& sc, response_hdr& rh, content_hdr& ch);
        /** Write the resouce as an output content stream.
         * Called by get(), post() on the child, and put().
         * Note that exceptions are ignored here because the response hdr
         * has already been sent.
         *
         * "hello, world" example, continued:
         * \code
         * *oc << "hello, world!";
         * \endcode
         *
         * Default is to do nothing (send no content). */
        virtual void write(ocontent::pointer oc);
    private:
        void setup();
        void respond(const version_type& v = version_type());
        int methods_allowed_mask_;
        bool chain_children_;
        bool default_response_;
        method_pointer method_;
        uri_info_type uri_info_;
        version_constraint_type version_constraint_;
        pointer child_;
        boost::weak_ptr<resource> parent_;
    };
    /** \brief Copy version constraint info from request hdr.
     *
     * Notes:<ul>
     * <li>If there are conflicting constraints the unmodified ones
     *     are chosen as they are the stronger and less likely to
     *     do damage (RFC does not give guidance).</li>
     * <li>Only similar constraints are paired. \c If-Match is paired
     *     with \c If-Unmodified-Since and \c If-None-Match with
     *     \c If-Modified-Since.</li>
     * <li>Multiple tags in \c If-Match and \c If-None-Match are
     *     not supported.</li>
     * <li>An \c If-Match of "*" is the same as no constraint since
     *     the resource was found and has some sort of tag and in
     *     this library methods are only applied to exising resources.</li>
     * <li>An \c If-None-Match of "*" is the unsatisfiable constraint,
     *     because in this library methods are only performed on
     *     existing resources which always have a tag. We leave "*"
     *     as is for version_constraint to handle.</li>
     * </ul> */
    void RESTCGI_API copy(const request_hdr& rh, version_constraint& vc);
    /** \brief Copy version info to response hdr. */
    void RESTCGI_API copy(const version& v, response_hdr& rh);
}
#endif
