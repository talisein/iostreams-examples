/*
author:  "Klaus Wittlich" <Klaus_Wittlich@sae.de>

Based on source code published in the book "Standard C++ IOStreams
and Locales" by Angelika Langer & Klaus Kreft, Copyright (c) 2000 by
Addison Wesley Longman, Inc.

Permission to use, copy, and modify this software for any non-profit
purpose is hereby granted without fee.  Neither the author of this
source code, Klaus Wittlich, nor the authors of the above mentioned
book, Angelika Langer and Klaus Kreft, nor the publisher, Addison
Wesley Longman, Inc., make any representations about the suitability of this
software for any purpose.  It is provided "as is" without express or
implied warranty.
*/

/*
#include "outbuf_236.h"
*/

#ifndef OUTBUF_H
#define OUTBUF_H

#include <cstring>

// p. 236 {{{
template <class charT, class Traits>
class outbuf : public basic_streambuf<charT, Traits>
{
    public:
        outbuf();
        ~outbuf();
    using char_type = charT;
    using traits_type = Traits;
    using int_type = typename Traits::int_type;
    using pos_type = typename Traits::pos_type;
    using off_type = typename Traits::off_type;

    protected:
        streamsize xsputn(const char_type *s, streamsize n);
        int_type overflow (int_type c = traits_type::eof());
        int sync();

    private:
        static const int bufSize = 16;
        char_type buffer[bufSize];

        int buffer_out();

        // prohibit copying assignment
        outbuf(const outbuf&);
        outbuf & operator = (const outbuf&);
};

template <class charT, class traits>
outbuf<charT, traits>::outbuf()
{
    this->setp(buffer, buffer + bufSize);
}
// }}}

// p. 237 {{{
template <class charT, class Traits>
outbuf<charT, Traits>::~outbuf()
{
    this->sync();
}

template <class charT, class Traits>
int outbuf<charT, Traits>::sync()
{
    return buffer_out();
}

template <class charT, class Traits>
int outbuf<charT, Traits>::buffer_out()
{
    int cnt = static_cast<int>(this->pptr() - this->pbase());
    int retval = buffer_to_device(buffer, cnt);

    this->pbump(-cnt);
    return retval;
}

template <class charT, class traits>
typename outbuf<charT,traits>::int_type
outbuf<charT,traits>::overflow (int_type c)
{
    if ( buffer_out() < 0 )
    {
        return traits_type::eof();
    }
// }}}
// p. 238 {{{
    else
    {
        if (!traits_type::eq_int_type (c, traits_type::eof() ) )
            return this->sputc(c);
        else
            return traits_type::not_eof(c);
    }
}

template <class charT, class Traits>
streamsize outbuf<charT, Traits>::xsputn(const char_type *s, streamsize n)
{
    if ( n < this->epptr() - this->pptr() )
    {
        memcpy(this->pptr(),s,n * sizeof(char_type));
        this->pbump(n);
        return n;
    }
    else
    {
        for ( streamsize i = 0; i<n; ++i )
        {
            if (traits_type::eq_int_type(this->sputc(s[i]), traits_type::eof()))
                return i;
        }
        return n;
    }
}
// }}}

#endif
