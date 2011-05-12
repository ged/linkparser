/*
 *  linkparser.c - Ruby LinkParser
 *  $Id: linkparser.c,v 21df914cedb1 2010/10/12 15:45:26 ged $
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
 * Raise a LinkParser::Error. The link-grammar library no longer supports fetching the actual
 * error message, so this just raises an exception with "Unknown error" now. Hopefully the 
 * library will have printed out the actual problem to stderr, and stderr is pointed
 * somewhere useful. 
 */
void
rlink_raise_lp_error() {
	rb_raise( rlink_eLpError, "Unknown error" );
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
 *  call-seq:
 *     LinkParser.link_grammar_version   -> string
 *
 *  Return the version of the link-grammar library the binding is linked against.
 *
 */
static VALUE
rlink_link_grammar_version( VALUE self ) {
#ifdef HAVE_LINKGRAMMAR_GET_VERSION
	const char *version = linkgrammar_get_version();
	if ( !version ) rb_bug( "linkgrammar_get_version returned NULL pointer" );
	return rb_str_new2( version );
#else
	return rb_str_new2( "link-grammar-4.3.9-or-earlier" );
#endif /* HAVE_LINKGRAMMAR_GET_VERSION */
}


/*
 * LinkParser extension init function
 */
void
Init_linkparser_ext() {
	rlink_mLinkParser = rb_define_module( "LinkParser" );

	/* The exception class used for LinkParser errors */
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );

	rb_define_singleton_method( rlink_mLinkParser, "link_grammar_version",
		rlink_link_grammar_version, 0 );

	setlocale( LC_ALL, "" );

	rlink_init_dict();
	rlink_init_sentence();
	rlink_init_linkage();
	rlink_init_parseoptions();
}

