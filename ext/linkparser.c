/*
 *  linkparser.c - Ruby LinkParser
 *  $Id$
 *  
 *  Authors:
 *    * Michael Granger <ged@FaerieMUD.org>
 *  
 *  Please see the LICENSE file at the top of the distribution for licensing 
 *  information.
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
	char buf[BUFSIZ], buf2[BUFSIZ];
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
rlink_raise_lp_error() {
	if ( lperrno ) {
		rb_raise( rlink_eLpError, lperrmsg );
	} else {
		rb_raise( rlink_eLpError, "Unknown error" );
	}
}


/* Make a Parse_Options after merging the specified default_options with any 
   new options given. */
VALUE
rlink_make_parse_options( VALUE default_options, VALUE options ) {
	if ( NIL_P(options) ) options = rb_hash_new();
	options = rb_funcall( default_options, rb_intern("merge"), 1, options );

	return rb_class_new_instance( 1, &options, rlink_cParseOptions );
}




/* 
 *  Namespace module for the Ruby LinkParser library
 */
void
Init_linkparser_ext() {
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );
	
	setlocale( LC_ALL, "" );

	rlink_init_dict();
	rlink_init_sentence();
	rlink_init_linkage();
	rlink_init_parseoptions();
}

