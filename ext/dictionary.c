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
 * Allocation function
 */
static struct rlink_dictionary *
rlink_dictionary_alloc()
{
	struct rlink_dictionary *ptr = ALLOC( struct rlink_dictionary );

	ptr->dict	= NULL;

	rlink_log( "debug", "Initialized an rlink_dictionary <%p>", ptr );
	return ptr;
}


/*
 * Free function
 */
static void
rlink_dict_gc_free( struct rlink_dictionary *ptr )
{
	if ( ptr ) {
		if ( ptr->dict )
			dictionary_delete( ptr->dict );

		ptr->dict = NULL;

		xfree( ptr );
		ptr = NULL;
	}
}


/*
 * Object validity checker. Returns the data pointer.
 */
static struct rlink_dictionary *
check_dict( VALUE self )
{
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
static struct rlink_dictionary *
get_dict( VALUE self )
{
	struct rlink_dictionary *ptr = check_dict( self );

	if ( !ptr )
		rb_raise( rb_eRuntimeError, "uninitialized Dictionary" );

	return ptr;
}


/* 
 * Get the Dictionary behind the LinkParser::Dictionary +object+ specified.
 */
struct rlink_dictionary *
rlink_get_dict( VALUE obj )
{
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
rlink_dict_s_alloc( VALUE klass )
{
	rlink_log( "debug", "Wrapping an uninitialized Dictionary pointer." );
	return Data_Wrap_Struct( klass, 0, rlink_dict_gc_free, 0 );
}


/*
 * Make a Dictionary with explicit datafile arguments. This is largely unnecessary, but
 * can be useful for testing and stuff.
 */
static Dictionary
rlink_make_oldstyle_dict( VALUE dict_file, VALUE pp_file, VALUE cons_file, VALUE affix_file )
{
#ifdef HAVE_DICTIONARY_CREATE
	SafeStringValue( dict_file  );
	SafeStringValue( pp_file    );
	SafeStringValue( cons_file  );
	SafeStringValue( affix_file );

	return dictionary_create(
		StringValuePtr(dict_file ),
		StringValuePtr(pp_file   ),
		StringValuePtr(cons_file ),
		StringValuePtr(affix_file)
	);
#else
	rb_raise( rb_eNotImpError,
		"Old-style dictionary creation isn't supported by the installed version of link-grammar." );
    UNREACHABLE;
#endif
}


/*
 *  call-seq:
 *      LinkParser::Dictionary.new( *args )                        -> dict
 *
 * Create a new LinkParser::Dictionary.
 * 
 * The preferred way to set up the dictionary is to call it with no
 * arguments, which will look for a dictionary with the same language
 * as the current environment. Alternatively, a fixed language can be
 * specified by specifying an ISO639 language code, for example,
 * <tt>LinkParser::Dictionary.new( :en )</tt>.
 * 
 * Explicit dictionary file names can be also specified, like so:
 *     
 *     Dictionary.new( dict_file,
 *                     post_process_file,
 *                     constituent_knowledge_file,
 *                     affix_file )
 *     
 * This mode of dictionary construction is not recommended for new 
 * development, and is intended for advanced users only. To create the 
 * dictionary, the Dictionary looks in the current directory and the data 
 * directory for the files +dict_file+, +post_process_file+,
 * +constituent_knowledge_file+, and +affix_file+. The last three entries
 * may be omitted. If +dict_file+ is a fully specified path name, then
 * the other file names, which need not be fully specified, will be
 * prefixed by the directory specified by +dict_file+.
 * 
 * In any case, a Hash of options can be specified which will be used
 * as default ParseOption attributes for any sentences created from
 * it.
 *
 *  Examples:
 *      dict = LinkParser::Dictionary.new
 *      
 *      dict = LinkParser::Dictionary.new( :de )
 *      
 *      dict = LinkParser::Dictionary.new( '/var/data/custom_dicts/4.2.dict' )
 *      
 * 
 */
static VALUE
rlink_dict_initialize( int argc, VALUE *argv, VALUE self )
{
	if ( !check_dict(self) ) {
		int i = 0;
		struct rlink_dictionary *ptr = NULL;
		Dictionary dict = NULL;
		VALUE arg1, arg2, arg3, arg4, arg5 = Qnil;
		VALUE lang = Qnil;
		VALUE opthash = Qnil;

		switch( i = rb_scan_args(argc, argv, "05", &arg1, &arg2, &arg3, &arg4, &arg5) ) {
		  /* Dictionary.new */
		  case 0:
			rlink_log_obj( self, "debug", "No arguments" );
			break;

		  /* Dictionary.new( lang )*/
		  /* Dictionary.new( opthash )*/
		  case 1:
			if( TYPE(arg1) == T_HASH ) {
				rlink_log_obj( self, "debug", "One arg: options hash." );
				opthash = arg1;
			} else {
				rlink_log_obj( self, "debug", "One arg: language" );
				lang = arg1;
			}
			break;

		  /* Dictionary.new( lang, opthash ) */
		  case 2:
			rlink_log_obj( self, "debug", "Two args: language and options hash." );
			lang = arg1;
			opthash = arg2;
			break;

		  /* Dictionary.new( dict, pp, cons, affix ) */
		  /* Dictionary.new( dict, pp, cons, affix, opthash ) */
		  case 4:
		  case 5:
			rlink_log_obj( self, "debug", "Four or five args: old-style explicit dict files." );
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
				dict = dictionary_create_lang( StringValuePtr(lang) );
			} else {
				dict = dictionary_create_default_lang();
			}
		}

		/* If the dictionary still isn't created, there was an error
		   creating it */
		if ( !dict ) rlink_raise_lp_error();

		rlink_log_obj( self, "debug", "Created dictionary %p", dict );
		DATA_PTR( self ) = ptr = rlink_dictionary_alloc();

		ptr->dict = dict;

		/* If they passed in an options hash, save it for later. */
		if ( RTEST(opthash) ) {
			rb_iv_set( self, "@options", opthash );
		} else {
			rb_iv_set( self, "@options", rb_hash_new() );
		}
	}

	else {
		rb_raise( rb_eRuntimeError, "Cannot re-initialize a Dictionary object." );
	}

	return Qnil;
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
rlink_parse( int argc, VALUE *argv, VALUE self )
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





/* 
 *  Document-class: LinkParser::Dictionary
 *  
 *  A Dictionary is the programmer's handle on the set of word definitions that defines the
 *  grammar. A user creates a Dictionary from a grammar file and post-process knowledge
 *  file, and then creates all other objects through it.
 */
void
rlink_init_dict()
{
#ifdef FOR_RDOC
	rlink_mLinkParser = rb_define_module( "LinkParser" );
#endif

	rlink_cDictionary = rb_define_class_under( rlink_mLinkParser, "Dictionary",
		rb_cObject );

	rb_define_alloc_func( rlink_cDictionary, rlink_dict_s_alloc );
	rb_define_method( rlink_cDictionary, "initialize", rlink_dict_initialize, -1 );

	rb_define_method( rlink_cDictionary, "parse", rlink_parse, -1 );

	/* The LinkParser::ParseOptions object for the Dictionary */
	rb_define_attr( rlink_cDictionary, "options", 1, 0 );
}

