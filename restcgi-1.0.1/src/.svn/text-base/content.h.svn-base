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
#ifndef restcgi_content_h
#define restcgi_content_h
#include "apidefs.h"
#include "hdr.h"
#include <string>
#include <boost/shared_ptr.hpp>
namespace restcgi {
    class env;
    class endpoint;
    /** \brief Base class for content stream (HTTP message body). */
    class RESTCGI_API content {
    public:
        typedef restcgi::content_hdr hdr_type; ///< header type
        virtual ~content();
        const hdr_type& hdr() const {return hdr_;} ///< Get header.
    protected:
        content(const boost::shared_ptr<endpoint>& ep, const hdr_type& h); ///< Construct.
        boost::shared_ptr<endpoint> endpoint_; ///< endpoint
    private:
        hdr_type hdr_;
    };
    /** \brief Input content stream from HTTP request message body.
     *
     * This is returned from the method object to read the request message body as
     * a stream. The content header fields (in the content base class) are the ones that
     * were in the request message.
     *
     * Note that the server may not send and end-of-file at the end of input.
     * See the hdr::find() method.
     * @see http://hoohoo.ncsa.uiuc.edu/cgi/in.html
     * @see method */
    class RESTCGI_API icontent : public content {
    public:
        typedef boost::shared_ptr<icontent> pointer; ///< shared ptr
        typedef boost::shared_ptr<const icontent> const_pointer; ///< const shared ptr
        typedef std::istream istream_type; ///< istream type
        ~icontent();
        /// Get input stream. See also stream operator free function.
        /// @see operator >>(icontent& ic, T& v)
        istream_type& istream();
    private:
        icontent(const icontent&); // inhibit
        icontent& operator =(const icontent&); // inhibit
        friend class method;
        icontent(const boost::shared_ptr<endpoint>& ep, const hdr_type& h);
    };
    /** \brief Stream in content. */
    template<typename T> icontent& operator >>(icontent& ic, T& v) {ic.istream() >> v; return ic;}
    /** \brief  Output content stream for HTTP response message body.
     *
     * This is returned
     * from the method object to write the response message body as a stream.
     * The content header fields (in the content base class) are the ones that
     * were used to create the response message (and this object).
     * @see method */
    class RESTCGI_API ocontent : public content {
    public:
        ~ocontent();
        typedef boost::shared_ptr<ocontent> pointer; ///< shared ptr
        typedef boost::shared_ptr<const ocontent> const_pointer; ///< const shared ptr
        typedef std::ostream ostream_type; ///< ostream type
        /// Get output stream. See also stream operator free function.
        /// @see operator <<(ocontent& oc, const T& v)
        ostream_type& ostream();
    private:
        ocontent(const ocontent&); // inhibit
        ocontent& operator =(const ocontent&); // inhibit
        friend class method;
        ocontent(const boost::shared_ptr<endpoint>& ep, const hdr_type& h);
    };
    /** \brief Stream out content. */
    template<typename T> ocontent& operator <<(ocontent& oc, const T& v) {oc.ostream() << v; return oc;}
    /** \brief Stream out content function. */
    inline ocontent& operator <<(ocontent& oc, std::ostream& (*v)(std::ostream&)) {oc.ostream() << v; return oc;}
}
#endif
