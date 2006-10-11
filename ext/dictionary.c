/*
 *  dict.c - Ruby LinkParser - Dictionary Class
 *  $Id$
 *  
 *  Authors:
 *    * Michael Granger <ged@FaerieMUD.org>
 *  
 *  Copyright (c) 2006 The FaerieMUD Consortium.
 *  
 *  This work is licensed under the Creative Commons Attribution License. To
 *  view a copy of this license, visit
 *  http://creativecommons.org/licenses/by/1.0 or send a letter to Creative
 *  Commons, 559 Nathan Abbott Way, Stanford, California 94305, USA.
 *  
 */

#include <stdlib.h>  /* for workaround_locale_bug() */

#include "linkparser.h"




/* --------------------------------------------------
 *  Forward declarations
 * -------------------------------------------------- */
void workaround_locale_bug();


/* --------------------------------------------------
 *  Memory management functions
 * -------------------------------------------------- */

/*
 * Free function
 */
static void
rlink_dict_gc_free( dict )
	Dictionary dict;
{
	if ( dict ) dictionary_delete( dict );
}


/*
 * Object validity checker. Returns the data pointer.
 */
static Dictionary
check_dict( self )
	 VALUE	self;
{
	debugMsg(( "Checking a LinkParser::Dictionary object (%d).", self ));
	Check_Type( self, T_DATA );

    if ( !IsDictionary(self) ) {
		rb_raise( rb_eTypeError, "wrong argument type %s (expected LinkParser::Dictionary)",
				  rb_class2name(CLASS_OF( self )) );
    }
	
	return DATA_PTR( self );
}


/*
 * Fetch the data pointer and check it for sanity.
 */
static Dictionary
get_dict( self )
	 VALUE self;
{
	Dictionary dict = check_dict( self );

	debugMsg(( "Fetching a Dictionary (%p).", dict ));
	if ( !dict )
		rb_raise( rb_eRuntimeError, "uninitialized Dictionary" );

	return dict;
}


/* 
 * Get the Dictionary behind the LinkParser::Dictionary +object+ specified.
 */ 
Dictionary
rlink_get_dict( obj )
	VALUE obj;
{
	return get_dict( obj );
}



/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 * allocate()
 * --
 * Allocate a new LinkParser::Dictionary object.
 */
static VALUE
rlink_dict_s_alloc( klass )
	 VALUE klass;
{
	debugMsg(( "Wrapping an uninitialized Dictionary pointer." ));
	return Data_Wrap_Struct( klass, 0, rlink_dict_gc_free, 0 );
}


static Dictionary
rlink_make_oldstyle_dict( dict_file, pp_file, cons_file, affix_file )
	VALUE dict_file, pp_file, cons_file, affix_file;
{
	SafeStringValue( dict_file  );
	SafeStringValue( pp_file    );
	SafeStringValue( cons_file  );
	SafeStringValue( affix_file );
	
	return dictionary_create(
		STR2CSTR( dict_file  ),
		STR2CSTR( pp_file    ),
		STR2CSTR( cons_file  ),
		STR2CSTR( affix_file )
	);
}


/*
 *  call-seq:
 *    LinkParser::Dictionary.new( options={} )
 *    LinkParser::Dictionary.new( language, options={} )
 *    LinkParser::Dictionary.new( dict, pp, ck, affix, option={} )
 * 
 *  Create a new LinkParser::Dictionary with data files for the given +language+, or
 *  using the specified data files.
 *
 *
 *
 */
static VALUE
rlink_dict_initialize( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
	if ( !check_dict(self) ) {
		int i = 0;
		Dictionary dict = NULL;
		VALUE arg1, arg2, arg3, arg4, arg5 = Qnil;
		VALUE lang = Qnil;
		VALUE opthash = Qnil;
		
		switch( i = rb_scan_args(argc, argv, "05", &arg1, &arg2, &arg3, &arg4, &arg5) ) {
		  /* Dictionary.new */
		  case 0:
			debugMsg(( "No arguments" ));
			break;

		  /* Dictionary.new( lang )*/
		  /* Dictionary.new( opthash )*/
		  case 1:
			if( TYPE(arg1) == T_HASH ) {
				debugMsg(( "One arg: options hash."));
				opthash = arg1;
			} else {
				debugMsg(( "One arg: language" ));
				lang = arg1;
			}
			break;

		  /* Dictionary.new( lang, opthash ) */
		  case 2:
			debugMsg(( "Two args: language and options hash."));
			lang = arg1;
			opthash = arg2;
			break;

		  /* Dictionary.new( dict, pp, cons, affix ) */
		  /* Dictionary.new( dict, pp, cons, affix, opthash ) */
		  case 4:
		  case 5:
			debugMsg(( "Four or five args: old-style explicit dict files." ));
			dict = rlink_make_oldstyle_dict( arg1, arg2, arg3, arg4, arg5 );
			break;
		
		  /* Anything else is an error */	
		  default:
			rb_raise( rb_eArgError, 
				"wrong number of arguments (%d for 0,1,2,4, or 5)", i );
		}

		/* Create the dictionary if it hasn't been already */
		if ( !dict && i < 4 ) {
			if ( RTEST(lang) ) {
				SafeStringValue( lang );
				dict = dictionary_create_lang( STR2CSTR(lang) );
			} else {
				workaround_locale_bug();
				dict = dictionary_create_default_lang();
			}
		}
		
		/* If the dictionary still isn't created, there was an error
		   creating it */
		if ( !dict ) rlink_raise_lp_error();

		DATA_PTR( self ) = dict;

		/* If they passed in an options hash, save it for later. */
		if ( RTEST(opthash) ) rb_iv_set( self, "@options", opthash );
		else rb_iv_set( self, "@options", rb_hash_new() );
	}

	else {
		rb_raise( rb_eRuntimeError, "Cannot re-initialize a Dictionary object." );
	}

	return Qnil;
}



/*
  The link-grammar library segfaults when you call
  dictionary_create_default_lang() with ENV['LANG'] unset, so just set it to
  English if it isn't already set.
*/
void
workaround_locale_bug()
{
	setenv( "LANG", "en", 0 );
}


/*
 * max_cost()
 * --
 * Returns the maximum cost (number of brackets []) that is placed on any
 * connector in the dictionary. This is useful for designing a parsing
 * algorithm that progresses in stages, first trying the cheap connectors.
 */
static VALUE 
rlink_get_max_cost( self )
	VALUE self;
{
	Dictionary dict = get_dict( self );
	int cost = dictionary_get_max_cost( dict );
	
	debugMsg(( "Max cost is: %d", cost ));
	
	return INT2NUM( cost );
}


/*
 * parse( sentence_string )
 * --
 * Parse the specified +sentence_string+ with the receiving Dictionary and
 * return a LinkParser::Sentence.
 */
static VALUE 
rlink_parse( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE input_string, options, sentence;
	VALUE args[2];
	int i;

	i = rb_scan_args( argc, argv, "11", &input_string, &options );

	/* Create the new sentence */
	args[0] = input_string;
	args[1] = self;
	sentence = rb_class_new_instance( 2, args, rlink_cSentence );
	
	/* Now call #parse on it */
	if ( i == 1 )
		rb_funcall( sentence, rb_intern("parse"), 0, 0 );
	else
		rb_funcall( sentence, rb_intern("parse"), 1, options );
	
	return sentence;
}





/* Initializer */
void
rlink_init_dict(void)
{
#ifdef FOR_RDOC
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );
#endif

	rlink_cDictionary = rb_define_class_under( rlink_mLinkParser, "Dictionary",
	 	rb_cObject );
	
	rb_define_alloc_func( rlink_cDictionary, rlink_dict_s_alloc );
	rb_define_method( rlink_cDictionary, "initialize", rlink_dict_initialize, -1 );

	rb_define_method( rlink_cDictionary, "max_cost", rlink_get_max_cost, 0 );
	rb_define_method( rlink_cDictionary, "parse", rlink_parse, -1 );

	rb_define_attr( rlink_cDictionary, "options", 1, 0 );
}

