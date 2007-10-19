/*
 *  linkparser.c - Ruby LinkParser
 *  $Id$
 *  
 *  Authors:
 *    * Michael Granger <ged@FaerieMUD.org>
 *  
 *  Copyright (c) 2007 The FaerieMUD Consortium
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *  
 */

#include "linkparser.h"

/*
 * Globals
 */

VALUE rlink_mLinkParser;

VALUE rlink_eLpError;

VALUE rlink_cDictionary;
VALUE rlink_cSentence;
VALUE rlink_cLinkage;
VALUE rlink_cParseOptions;

VALUE rlink_sLinkageCTree;


/* --------------------------------------------------
 * Utility functions
 * -------------------------------------------------- */

void
#ifdef HAVE_STDARG_PROTOTYPES
rlink_debug(const char *fmt, ...)
#else
rlink_debug(fmt, va_alist)
	 const char *fmt;
	 va_dcl
#endif
{
	char		buf[BUFSIZ], buf2[BUFSIZ];
	va_list	args;

	if (!RTEST(ruby_debug)) return;

	snprintf( buf, BUFSIZ, "LinkParser Debug>>> %s", fmt );

	va_init_list( args, fmt );
	vsnprintf( buf2, BUFSIZ, buf, args );
	fputs( buf2, stderr );
	fputs( "\n", stderr );
	fflush( stderr );
	va_end( args );
}


/*
 * Raise a LinkParser::Error, either with the message in lperrmsg if it's
 * set or a generic error if not.
 */
void
rlink_raise_lp_error( void )
{
	if ( lperrno ) {
		rb_raise( rlink_eLpError, lperrmsg );
	} else {
		rb_raise( rlink_eLpError, "Unknown error" );
	}
}


/* Make a Parse_Options after merging the specified default_options with any 
   new options given. */
VALUE
rlink_make_parse_options( default_options, options )
	VALUE default_options, options;
{
	if ( NIL_P(options) ) options = rb_hash_new();
	options = rb_funcall( default_options, rb_intern("merge"), 1, options );

	return rb_class_new_instance( 1, &options, rlink_cParseOptions );
}




/* Namespace module for the Ruby LinkParser library */
void
Init_linkparser_ext(void)
{
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );
	
	rlink_init_dict();
	rlink_init_sentence();
	rlink_init_linkage();
	rlink_init_parseoptions();
}

