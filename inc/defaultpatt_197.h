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
#include "defaultpatt_197.h"
*/

#ifndef DEFAULTPATT_H
#define DEFAULTPATT_H

// p. 197 {{{
template <class charT, class Traits>
const basic_string<charT> defaultPatt(const basic_ostream<charT, Traits>& str)
{
    static char const * const c_helper = "%x";
    static charT helper[3] = {charT('\0'), charT('\0'), charT('\0') };

    use_facet<ctype<charT> > (str.getloc()).widen(c_helper, c_helper+2, helper);

    return helper;
}
// }}}

#endif
