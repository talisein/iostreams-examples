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
#include "setfmt_helper_221.h"
*/

#ifndef SETFMT_HELPER_203_H
#define SETFMT_HELPER_203_H

#include "one_arg_manip_weh_187.h"
#include "getidx_195.h"


// p. 203 {{{
template <class charT>
class setfmt_helper : public one_arg_manip_weh<const charT*>
{
    public:
        setfmt_helper(const charT* fmt)
            : one_arg_manip_weh<const charT*>(setfmt_fct,fmt) { }
// }}}
// p. 221 {{{
	static void setfmt_fct(ios_base& str, const charT* f)
	{
		datefmt<charT> * p;
		if (( p = dynamic_cast<datefmt<charT>*> (&str)) != 0)
			p->fmt(f);
	}
// }}}
// p. 205 {{{
// Implementation from p. 205
	static void callback(ios_base::event ev, ios_base& str, int i)
    {
        if ( ev == ios_base::erase_event )
        {
            try {delete static_cast<basic_string<charT>*> (str.pword(i)); }
            catch (...) {}
        }
// }}}
// p. 206 {{{
        else if (ev == ios_base::copyfmt_event )
        {
            void *& formatStringPtr = str.pword(i);
            basic_string<charT> * old;
            if ( (old = static_cast<basic_string<charT>*> (formatStringPtr)) != 0 )
            {
                try { formatStringPtr = new basic_string<charT>(*old); }
                catch (bad_alloc&)
				{
					ios_base::iostate err = ios_base::badbit;
					str.iword(i) = err | static_cast<ios_base::iostate> (str.iword(i));
				}
            }
        }
    }
// }}}
// p. 204 {{{
};
// }}}

// p. 203 {{{
template <class charT>
setfmt_helper<charT> setfmt(const charT * str)
{
    return setfmt_helper<charT>(str);
}
// }}}

#endif
