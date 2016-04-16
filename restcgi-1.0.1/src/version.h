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
#ifndef restcgi_version_h
#define restcgi_version_h
#include "apidefs.h"
#include "date_time.h"
#include "method_e.h"
#include <string>
#ifdef _WIN32
#pragma warning (disable: 4251)
#endif
namespace restcgi {
    /** \brief Version tag class for identifying versions using a string.
     *
     * The constructor takes different types and converts them
     * to string for ease of use. Note that tags need only be unique wrt one
     * resource. The simplest tag syntax is a number that increases
     * each time the resource is modified.
     *
     * This is used as the \c ETag string for constraint comparisons
     * and reporting the resource's tag.
     * @see version
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.19 */
    class RESTCGI_API version_tag {
    public:
        version_tag(); ///< Construct null.
        version_tag(const std::string& v); ///< Construct from string.
        version_tag(int v); ///< Construct from int.
        version_tag(unsigned int v); ///< Construct from unsigned int.
        version_tag(long long v); ///< Construct from long long.
        version_tag(unsigned long long v); ///< Construct from unsigned long long.
        bool is_null() const {return string_.empty();} ///< Test if null.
        const std::string& string() const {return string_;} ///< Get string.
        /// Equal operator.
        bool operator ==(const version_tag& rhs) const {return string_ == rhs.string_;}
        /// Not equal operator.
        bool operator !=(const version_tag& rhs) const {return string_ != rhs.string_;}
    private:
        std::string string_;
    };
    /** \brief Information that identifies a version of a resource.
     *
     * Identification is with
     * a version "tag" (version_tag) and/or a modification date-time.
     * This version functionality connects resource modifications to the
     * HTTP caching and operation contraint mechanisms. The version "tag"
     * is used directly as the HTTP \c ETag, for example, in the \c ETag
     * or \c If-None-Match header fields. The modification date is used in
     * the \c Last-Modified and \c If-Modified-Since fields.
     *
     * Applications should be designed to assign a verion tag to each
     * resource modification.
     *
     * For HTTP constraints to operate correctly, the application must
     * send either the modification date and/or the tag when replying
     * with resource content. Constraints can be checked automatically or
     * explicitly by the application prior to responding to a method.
     * @see version_constraint, version_tag */
    class RESTCGI_API version {
    public:
        typedef restcgi::version_tag tag_type; ///< tag type
        typedef restcgi::date_time date_time_type; ///< date-time type
        version(); ///< Construct.
        version(const tag_type& tag, const date_time_type& dt = date_time_type()); ///< Construct.
        version(const date_time_type& dt, const tag_type& tag = tag_type()); ///< Construct.
        bool is_null() const {return tag_.is_null() && date_time_.is_null();} ///< Test if null.
        const tag_type& tag() const {return tag_;} ///< Get tag.
        date_time_type date_time() const {return date_time_;} ///< Get modification date-time.
    private:
        tag_type tag_;
        date_time_type date_time_;
    };
    /** \brief Constraint on a resource's version which governs the operation of
     * or response to a method.
     *
     * A method request may contain a constraint
     * on the version of the resource to which it applies. There are 2
     * kinds of constraints: the resource's version must be identical to
     * the constraint's (the resource is unmodified) or the resource must have
     * a different version than the constraint's (the resource has been modified).
     * @see version, copy(const request_hdr& rh, version_constraint& vc)
     * @see http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.26 */
    class RESTCGI_API version_constraint {
    public:
        typedef restcgi::version version_type; ///< version type
        version_constraint(); ///< Construct.
        version_constraint(const version_type& v, bool mod); ///< Construct.
        bool is_null() const {return version_.is_null();} ///< Test if null.
        const version_type& version() const {return version_;} ///< Get version.
        bool modified() const {return modified_;} ///< Get whether constrained to be modified.
        /// Assert that the given version satisfies the constraint. This will
        /// check the version tags (if any) first then the expiration dates
        /// (if any). See class description.
        /// @exception precondition_failed unmodified constraint not met or modified
        ///     tag constraint not met
        /// @exception not_modified modified date-time constraint not met
        void assert_satisfied(const version_type& v, const method_e& e) const;
    private:
        version_type version_;
        bool modified_;
    };
}
#endif
