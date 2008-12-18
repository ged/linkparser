/*
 *  dict.c - Ruby LinkParser - Dictionary Class
 *  $Id$
 *  
 *  Authors:
 *    * Michael Granger <ged@FaerieMUD.org>
 *  
 *  Please see the LICENSE file at the top of the distribution for licensing 
 *  information.
 */

#include "linkparser.h"


/* --------------------------------------------------
 *  Memory management functions
 * -------------------------------------------------- */

/*
 * Free function
 */
static void
rlink_dict_gc_free( Dictionary dict ) {
	if ( dict ) dictionary_delete( dict );
}


/*
 * Object validity checker. Returns the data pointer.
 */
static Dictionary
check_dict( VALUE self ) {
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
get_dict( VALUE self ) {
	Dictionary dict = check_dict( self );

	if ( !dict )
		rb_raise( rb_eRuntimeError, "uninitialized Dictionary" );

	return dict;
}


/* 
 * Get the Dictionary behind the LinkParser::Dictionary +object+ specified.
 */ 
Dictionary
rlink_get_dict( VALUE obj ) {
	return get_dict( obj );
}



/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */


/*
 *  call-seq:
 *     LinkParser::Dictionary.allocate   -> dict
 *
 *  Allocate a new LinkParser::Dictionary object.
 */
static VALUE
rlink_dict_s_alloc( VALUE klass ) {
	debugMsg(( "Wrapping an uninitialized Dictionary pointer." ));
	return Data_Wrap_Struct( klass, 0, rlink_dict_gc_free, 0 );
}


/*
 * Make a Dictionary with explicit datafile arguments. This is largely unnecessary, but
 * can be useful for testing and stuff.
 */
static Dictionary
rlink_make_oldstyle_dict( VALUE dict_file, VALUE pp_file, VALUE cons_file, VALUE affix_file ) {
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
 */
static VALUE
rlink_dict_initialize( int argc, VALUE *argv, VALUE self ) {
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
			dict = rlink_make_oldstyle_dict( arg1, arg2, arg3, arg4 );
			opthash = arg5;
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
 *  call-seq:
 *     dictionary.max_cost   -> fixnum
 *
 *  Returns the maximum cost (number of brackets []) that is placed on any
 *  connector in the dictionary. This is useful for designing a parsing
 *  algorithm that progresses in stages, first trying the cheap connectors.
 */
static VALUE 
rlink_get_max_cost( VALUE self ) {
	Dictionary dict = get_dict( self );
	int cost = dictionary_get_max_cost( dict );
	
	debugMsg(( "Max cost is: %d", cost ));
	
	return INT2NUM( cost );
}


/*
 *  call-seq:
 *     dictionary.parse( string )            -> sentence
 *     dictionary.parse( string, options )   -> sentence
 *
 *  Parse the specified sentence +string+ with the dictionary and return a 
 *  LinkParser::Sentence. If you specify an +options+ hash, its values will override
 *  those of the Dictionary's for the resulting Sentence.
 */
static VALUE 
rlink_parse( int argc, VALUE *argv, VALUE self ) {
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





/* 
 *  Document-class: LinkParser::Dictionary
 *  
 *  A Dictionary is the programmer's handle on the set of word definitions that defines the
 *  grammar. A user creates a Dictionary from a grammar file and post-process knowledge
 *  file, and then creates all other objects through it.
 */
void
rlink_init_dict() {
#ifdef FOR_RDOC
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	
	/* An exception class for errors raised from the LinkParser library */
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

