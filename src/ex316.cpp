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

#include <locale>
#include <iostream>

using namespace ::std;

#include "x_facet_317.h"

locale::id base_facet::id;

// p. 316 {{{
template <class Facet>
const Facet& use_facet(const locale& loc)
{
	const locale::facet * pb;
	const Facet *pd;

	// use the Facet identification
	if ((pb = &std::use_facet<Facet>(loc))) // !!! use_facet insted get_facet TODO: there's no get_facet. It seems implementing use_facet is a secret in gcc
		throw (bad_cast()); // TODO: Can't define what

	// use the facet type
	if ((pd = dynamic_cast<const Facet*>(pb)) == 0)
		throw(bad_cast());// TODO: Can't define what

	return (*pd);
}
// }}}

int main()
{
	locale help;

	{
		locale loc(help, new base_facet);

		// p. 317 {{{
		cout << ::use_facet<base_facet>(loc).bar(); // !!! :: added to use function from book
		// }}}
		cout << endl;
	}

	{
		locale loc(help, new derived_facet);
		// p. 318 {{{
		cout << ::use_facet<base_facet>(loc).bar(); // !!! :: added to use function from book
		// }}}
		cout << endl;

		// p. 318 {{{
		cout << ::use_facet<derived_facet>(loc).bar_2(); // !!! :: added to use function from book
		// }}}
		cout << endl;
	}
	return 0;
}

/* Input: No input
*/
/* Output:
this is the base class
this is the derived class
hello world
*/
