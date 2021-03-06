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
#include "date_160.h"
*/

#ifndef DATE_H
#define DATE_H

// p. 148 {{{
class date {
public:
    date(int d, int m, int y)
    {
        tm_date.tm_mday = d;tm_date.tm_mon = m-1; tm_date.tm_year = y-1900;
        tm_date.tm_sec = tm_date.tm_min = tm_date.tm_hour = 0;
        tm_date.tm_wday = tm_date.tm_yday = 0;
        tm_date.tm_isdst = 0;
		mktime(&tm_date); // !!!
    }
    date(const tm& t) : tm_date(t) {}
    date()
    {
        time_t ltime;
        time(&ltime);
        tm_date = *localtime(&ltime);
    }
    // more constructors and useful member functions

private:
    tm tm_date;

    template<class charT, class Traits>
    friend basic_istream<charT, Traits>&
    operator >>(basic_istream<charT, Traits>& is, date& dat);

    template<class charT, class Traits>
    friend basic_ostream<charT, Traits>&
    operator<< (basic_ostream<charT, Traits>& os, const date& dat);
};
// }}}

// p. 159 {{{
template<class charT, class Traits>
basic_istream<charT, Traits>&
operator >>(basic_istream<charT, Traits>& is, date& dat)
{
	if (!is.good() )
		return is;

	ios_base::iostate err = ios_base::goodbit;
	typename basic_istream<charT, Traits>::sentry ipfx(is);
	if ( ipfx )
	{
		use_facet<time_get<charT, istreambuf_iterator<charT, Traits> > >(is.getloc() )
			.get_date(is, istreambuf_iterator<charT, Traits>(), is, err, &dat.tm_date);
	}

	return is;
}
// }}}

// p. 160 {{{
template<class charT, class Traits>
basic_ostream<charT, Traits>&
operator <<(basic_ostream<charT, Traits>& os, const date& dat)
{
    if (!os.good() ) {
        return os;
    }

	typename basic_ostream<charT, Traits>::sentry opfx(os);
	if (opfx)
	{
		basic_ostringstream<charT, Traits> sb;

		use_facet<time_put<charT, ostreambuf_iterator<charT, Traits> > >(os.getloc() )
			.put(sb, sb, os.fill(), &dat.tm_date,'x');

		basic_string<charT, Traits> s = sb.str();
		streamsize charToPad = static_cast<streamsize>(os.width() - s.length());
		ostreambuf_iterator<charT, Traits> sink(os);
		if (charToPad <= 0)
		{
			sink = copy(s.begin(), s.end(), sink);
		}
		else
		{
			if (os.flags() & ios_base::left)
			{
				sink = copy(s.begin(), s.end(), sink);
				fill_n(sink,charToPad,os.fill());
			}
			else
			{
				fill_n(sink,charToPad,os.fill());
				sink = copy(s.begin(), s.end(), sink);
			}
		}
	}
	os.width(0);
	return os;
}
// }}}

#endif
