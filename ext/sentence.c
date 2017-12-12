/*
 *  sentence.c - Ruby LinkParser
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
 *  Forward declarations
 * -------------------------------------------------- */


/* --------------------------------------------------
 * Macros and constants
 * -------------------------------------------------- */


/* --------------------------------------------------
 *	Memory-management functions
 * -------------------------------------------------- */

/*
 * Allocation function
 */
static struct rlink_sentence *
rlink_sentence_alloc()
{
	struct rlink_sentence *ptr = ALLOC( struct rlink_sentence );

	ptr->sentence	= NULL;
	ptr->dictionary	= Qnil;
	ptr->parsed_p	= Qfalse;
	ptr->options	= Qnil;

	rlink_log( "debug", "Initialized an rlink_sentence <%p>", ptr  );
	return ptr;
}


/*
 * GC Mark function
 */
static void
rlink_sentence_gc_mark( struct rlink_sentence *ptr )
{
	if ( ptr ) {
		rb_gc_mark( ptr->dictionary );
		rb_gc_mark( ptr->options );
	}
}


/*
 * GC Free function
 */
static void
rlink_sentence_gc_free( struct rlink_sentence *ptr )
{
	if ( ptr ) {
		if ( ptr->dictionary && TYPE(ptr->dictionary) == T_DATA ) {
			struct rlink_dictionary *dictionary = rlink_get_dict( ptr->dictionary );

			/* Freeing the dictionary automatically frees the sentences it belongs to, so
			   don't double-free if the dictionary struct or its pointer is done. */
			if ( dictionary->dict ) {
				sentence_delete( (Sentence)ptr->sentence );
			}
		}

		ptr->sentence = NULL;
		ptr->options = Qnil;
		ptr->dictionary = Qnil;

		xfree( ptr );
		ptr = NULL;
	}
}


/*
 * Object validity checker. Returns the data pointer.
 */
static struct rlink_sentence *
check_sentence(  VALUE	self )
{
	Check_Type( self, T_DATA );

    if ( !IsSentence(self) ) {
		rb_raise( rb_eTypeError, "wrong argument type %s (expected LinkParser::Sentence)",
				  rb_class2name(CLASS_OF( self )) );
    }

	return DATA_PTR( self );
}


/*
 * Fetch the data pointer and check it for sanity.
 */
static struct rlink_sentence *
get_sentence(  VALUE self )
{
	struct rlink_sentence *ptr = check_sentence( self );

	if ( !ptr )
		rb_raise( rb_eRuntimeError, "uninitialized Sentence" );

	return ptr;
}


/*
 * Publicly-usable sentence-fetcher
 */
struct rlink_sentence *
rlink_get_sentence( VALUE self )
{
	return get_sentence( self );
}



/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 *  call-seq:
 *     LinkParser::Sentence.allocate   -> sentence
 *
 *  Allocate a new LinkParser::Sentence object.
 *
 */
static VALUE
rlink_sentence_s_alloc(  VALUE klass )
{
	rlink_log( "debug", "Wrapping an uninitialized Sentence pointer." );
	return Data_Wrap_Struct( klass, rlink_sentence_gc_mark, rlink_sentence_gc_free, 0 );
}


/* --------------------
 * Instance methods
 * -------------------- */

/*
 *  call-seq:
 *     LinkParser::Sentence.new( str, dict )   -> sentence
 *
 *  Create a new LinkParser::Sentence object from the given input string
 *  using the specified LinkParser::Dictionary.
 *
 *     dict = LinkParser::Dictionary.new
 *     LinkParser::Sentence.new( "The boy runs", dict )  #=> #<LinkParser::Sentence:0x5481ac>
 */
static VALUE
rlink_sentence_init( VALUE self, VALUE input_string, VALUE dictionary )
{
	if ( !check_sentence(self) ) {
		struct rlink_sentence *ptr;
		Sentence sent;
		struct rlink_dictionary *dictptr = rlink_get_dict( dictionary );

		if ( !(sent = sentence_create( StringValueCStr(input_string), dictptr->dict )) )
			rlink_raise_lp_error();

		DATA_PTR( self ) = ptr = rlink_sentence_alloc();

		ptr->sentence = sent;
		ptr->dictionary = dictionary;
		ptr->options = Qnil;

	} else {
		rb_raise( rb_eRuntimeError,
				  "Cannot re-initialize a sentence once it's been created." );
	}

	return self;
}


/*
 *  call-seq:
 *     sentence.parse( options={} )   -> fixnum
 *
 *  Attach a parse set to this sentence and return the number of linkages
 *  found. If any +options+ are specified, they override those set in the
 *  sentence's dictionary.
 *
 */
static VALUE
rlink_sentence_parse( int argc, VALUE *argv, VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	Parse_Options opts;
	VALUE defopts = Qnil;
	VALUE options = Qnil;
	int link_count = 0;

	/*
	if ( RTEST(ptr->parsed_p) )
		rb_raise( rlink_eLpError, "Can't reparse a sentence." );
	*/
	rlink_log_obj( self, "debug", "Parsing sentence <%p>", ptr  );

	/* Merge the hash from this call with the one from the dict and build
	   Parse_Options from it. */
	rb_scan_args( argc, argv, "01", &options );
	defopts = rb_funcall( ptr->dictionary, rb_intern("options"), 0 );

	/* Turn the option hash into a ParseOptions object, then extract the
	   Parse_Options struct from that  */
	options = rlink_make_parse_options( defopts, options );
	opts = rlink_get_parseopts( options );

	/* Parse the sentence */
	if ( (link_count = sentence_parse( ptr->sentence, opts )) < 0 )
		rlink_raise_lp_error();

	ptr->options = options;
	ptr->parsed_p = Qtrue;

	return INT2FIX( link_count );
}


/*
 *  call-seq:
 *     sentence.parsed?   -> true or false
 *
 *  Returns +true+ if the sentence has been parsed.
 *
 *     sentence.parsed?   #-> false
 *     sentence.parse     #-> 6
 *     sentence.parsed?   #-> true
 */
static VALUE
rlink_sentence_parsed_p( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	return ptr->parsed_p;
}


/*
 *  call-seq:
 *     sentence.options   -> parseoptions
 *
 *  Returns a ParseOptions object for the receiving sentence.
 *
 *     sentence.options.verbosity = 3
 *     sentence.options.islands_ok?  # -> true
 */
static VALUE
rlink_sentence_options( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	return ptr->options;
}




/*
 *  call-seq:
 *     sentence.linkages   -> array
 *
 *  Returns an Array of LinkParser::Linkage objects which represent the
 *  parts parsed from the sentence for the current linkage.
 *
 */
static VALUE
rlink_sentence_linkages( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int i, count = 0;
	VALUE rary;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_num_valid_linkages( (Sentence)ptr->sentence );
	rary = rb_ary_new2( count );

	for ( i = 0; i < count; i++ ) {
		VALUE linkage;
		VALUE args[2];

		args[0] = INT2FIX( i );
		args[1] = self;

		linkage = rb_class_new_instance( 2, args, rlink_cLinkage );
		rb_ary_store( rary, i, linkage );
	}

	return rary;
}


/*
 *  call-seq:
 *     sentence.length   -> fixnum
 *
 *  Returns the number of words in the tokenized sentence, including the
 *  boundary words and punctuation.
 *
 */
static VALUE
rlink_sentence_length( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	return INT2FIX( sentence_length((Sentence)ptr->sentence) );
}


/*
 *  call-seq:
 *     sentence.null_count   -> int
 *
 *  Returns the number of null links that were used in parsing the sentence.
 */
static VALUE
rlink_sentence_null_count( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int count;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_null_count( (Sentence)ptr->sentence );
	return INT2FIX( count );
}


/*
 *  call-seq:
 *     sentence.num_linkages_found   -> fixnum
 *
 *  Returns the number of linkages found when parsing the sentence. This will
 *  cause the sentence to be parsed if it hasn't been already.
 */
static VALUE
rlink_sentence_num_linkages_found( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int i = 0;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	i = sentence_num_linkages_found( (Sentence)ptr->sentence );

	return INT2FIX( i );
}


/*
 *  call-seq:
 *     sentence.num_valid_linkages   -> fixnum
 *
 *  Return the number of linkages that had no post-processing violations.
 */
static VALUE
rlink_sentence_num_valid_linkages( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int count;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_num_valid_linkages( (Sentence)ptr->sentence );
	return INT2FIX( count );
}


/*
 *  call-seq:
 *     sentence.num_linkages_post_processed   -> fixnum
 *
 *  Return the number of linkages that were actually post-processed (which may
 *  be less than the number found because of the linkage_limit parameter).
 */
static VALUE
rlink_sentence_num_linkages_post_processed( VALUE self )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int count;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_num_linkages_post_processed( (Sentence)ptr->sentence );
	return INT2FIX( count );
}


/*
 *  call-seq:
 *     sentence.num_violations( i )   -> fixnum
 *
 *  The number of post-processing violations that the i-th linkage had during
 *  the last parse.
 */
static VALUE
rlink_sentence_num_violations( VALUE self, VALUE i )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int count;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_num_violations( (Sentence)ptr->sentence, FIX2INT(i) );
	return INT2FIX( count );
}


/*
 *  call-seq:
 *     sentence.disjunct_cost( i )   -> fixnum
 *
 *  The maximum cost of connectors used in the i-th linkage of the sentence.
 */
static VALUE
rlink_sentence_disjunct_cost( VALUE self, VALUE i )
{
	struct rlink_sentence *ptr = get_sentence( self );
	int count;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_disjunct_cost( (Sentence)ptr->sentence, FIX2INT(i) );
	return INT2FIX( count );
}


/*
 * Document-class: LinkParser::Sentence
 *
 *   A Sentence is the API's representation of an input string,
 *   tokenized and interpreted according to a specific Dictionary. After
 *   a Sentence is created and parsed, various attributes of the
 *   resulting set of linkages can be obtained.
 *
 */
void
rlink_init_sentence()
{
	rlink_cSentence = rb_define_class_under( rlink_mLinkParser, "Sentence",
		rb_cObject );

	rb_define_alloc_func( rlink_cSentence, rlink_sentence_s_alloc );

	rb_define_method( rlink_cSentence, "initialize", rlink_sentence_init, 2 );
	rb_define_method( rlink_cSentence, "parse", rlink_sentence_parse, -1 );
	rb_define_method( rlink_cSentence, "parsed?", rlink_sentence_parsed_p, 0 );
	rb_define_method( rlink_cSentence, "linkages", rlink_sentence_linkages, 0 );

	rb_define_method( rlink_cSentence, "options", rlink_sentence_options, 0 );

	rb_define_method( rlink_cSentence, "length", rlink_sentence_length, 0 );

	rb_define_method( rlink_cSentence, "null_count",
		rlink_sentence_null_count, 0 );
	rb_define_method( rlink_cSentence, "num_linkages_found",
		rlink_sentence_num_linkages_found, 0 );
	rb_define_method( rlink_cSentence, "num_valid_linkages",
		rlink_sentence_num_valid_linkages, 0 );
	rb_define_method( rlink_cSentence, "num_linkages_post_processed",
		rlink_sentence_num_linkages_post_processed, 0 );
	rb_define_method( rlink_cSentence, "num_violations",
		rlink_sentence_num_violations, 1 );
	rb_define_method( rlink_cSentence, "disjunct_cost",
		rlink_sentence_disjunct_cost, 1 );
}

