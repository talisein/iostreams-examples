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
#include "inbuf_239.h"
*/

#ifndef INBUF_H
#define INBUF_H

#include <cstring>

// p. 239 {{{
template <class charT, class Traits>
class inbuf : public basic_streambuf<charT, Traits>
{
    public:
        inbuf();
    using char_type = charT;
    using traits_type = Traits;
    using int_type = typename Traits::int_type;
    using pos_type = typename Traits::pos_type;
    using off_type = typename Traits::off_type;

    protected:
        int_type underflow();
        int_type pbackfail(int_type c);

    private:
        static const streamsize bufSize = 16;
        static const streamsize pbSize;
        char_type buffer[bufSize];
// }}}
// p. 240 {{{
        int buffer_in();

        // prohibit copying and assignment
        inbuf(const inbuf&);
        inbuf& operator=(const inbuf&);
};

template <class charT, class traits>
inbuf<charT,traits>::inbuf()
{
    this->setg(buffer+inbuf<charT,traits>::pbSize, buffer+inbuf<charT,traits>::pbSize, buffer+inbuf<charT,traits>::pbSize);
}


template <class charT, class traits>
typename inbuf<charT,traits>::int_type inbuf<charT,traits>::underflow()
{
    if(this->gptr() < this->egptr() )
        return traits_type::to_int_type(*this->gptr());

    if (buffer_in() < 0)
        return traits_type::eof();
// }}}
// p. 241 {{{
    else
        return traits_type::to_int_type(*this->gptr());
}


template <class charT, class traits>
int inbuf<charT,traits>::buffer_in()
{
  // determine number of putback characters
    streamsize numPutbacks = min(static_cast<long>(this->gptr() - this->eback()), inbuf<charT,traits>::pbSize);

    memmove(buffer + (inbuf<charT,traits>::pbSize-numPutbacks) * sizeof(char_type),
          this->gptr() - numPutbacks * sizeof(char_type),
         numPutbacks * sizeof(char_type));

  int retval = buffer_from_device(buffer + inbuf<charT,traits>::pbSize * sizeof(char_type),
                                  inbuf<charT,traits>::bufSize - inbuf<charT,traits>::pbSize);
  if (retval <= 0)
  {
    this->setg(0,0,0);
    return -1;
  }
  else
  {
    this->setg(buffer + inbuf<charT,traits>::pbSize - numPutbacks,
    buffer + inbuf<charT,traits>::pbSize, buffer + inbuf<charT,traits>::pbSize + retval);
    return retval;
  }
}
// }}}}

// p. 242 {{{
template <class charT, class traits>
typename inbuf<charT, traits>::int_type
inbuf<charT,traits>::pbackfail(int_type c)
{
    if (this->gptr() != this->eback())
    {
        this->gbump(-1);
        if (!traits_type::eq_int_type(c, traits_type::eof() ) )
            *(this->gptr()) = traits_type::not_eof(c);
        return traits_type::not_eof(c);
    }
    else
        return traits_type::eof();
}
// }}}

#endif
