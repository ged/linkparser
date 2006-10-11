/*
 *  sentence.c - Ruby LinkParser
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
static rlink_SENTENCE *
rlink_sentence_alloc()
{
	rlink_SENTENCE *ptr = ALLOC( rlink_SENTENCE );
	
	ptr->sentence	= NULL;
	ptr->dictionary	= Qnil;
	ptr->parsed_p	= Qfalse;
	ptr->options	= Qnil;
	
	debugMsg(( "Initialized an rlink_SENTENCE <%p>", ptr ));
	return ptr;
}


/*
 * GC Mark function
 */
static void
rlink_sentence_gc_mark( ptr )
	rlink_SENTENCE *ptr;
{
	debugMsg(( "Marking LinkParser::Sentence %p", ptr ));
	
	if ( ptr ) {
		rb_gc_mark( ptr->dictionary );
		rb_gc_mark( ptr->options );
	}
	
	else {
		debugMsg(( "Not marking uninitialized rlink_SENTENCE" ));
	}
}


/*
 * GC Free function
 */
static void
rlink_sentence_gc_free( ptr )
	rlink_SENTENCE *ptr;
{
	if ( ptr ) {
		debugMsg(( "In free function of Sentence <%p>", ptr ));
		
		if ( rlink_get_dict(ptr->dictionary) ) {
			debugMsg(( "Freeing Sentence <%p>", ptr->sentence ));
			sentence_delete( (Sentence)ptr->sentence );
		} else {
			debugMsg(( "Not freeing a Sentence belonging to an already-freed dictionary." ));
		}

		ptr->sentence = NULL;
		ptr->options = Qnil;
		ptr->dictionary = Qnil;
	}
	
	else {
		debugMsg(( "Not freeing an uninitialized rlink_SENTENCE" ));
	}
}


/*
 * Object validity checker. Returns the data pointer.
 */
static rlink_SENTENCE *
check_sentence( self )
	 VALUE	self;
{
	debugMsg(( "Checking a LinkParser::Sentence object (%d).", self ));
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
static rlink_SENTENCE *
get_sentence( self )
	 VALUE self;
{
	rlink_SENTENCE *ptr = check_sentence( self );

	debugMsg(( "Fetching a Sentence (%p).", ptr ));
	if ( !ptr )
		rb_raise( rb_eRuntimeError, "uninitialized Sentence" );

	return ptr;
}


/*
 * Publicly-usable sentence-fetcher
 */
rlink_SENTENCE *
rlink_get_sentence( self )
	VALUE self;
{
	return get_sentence( self );
}



/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 *  call-seq:
 *     LinkParser::Sentence.allocate   => sentence
 *
 *  Allocate a new LinkParser::Sentence object.
 *
 */
static VALUE
rlink_sentence_s_alloc( klass )
	 VALUE klass;
{
	debugMsg(( "Wrapping an uninitialized Sentence pointer." ));
	return Data_Wrap_Struct( klass, rlink_sentence_gc_mark, rlink_sentence_gc_free, 0 );
}


/* --------------------
 * Instance methods
 * -------------------- */

/*
 *  call-seq:
 *     LinkParser::Sentence.new( str, dict )   => sentence
 *
 *  Create a new LinkParser::Sentence object from the given input string
 #  using the specified LinkParser::Dictionary.
 *
 *     dict = LinkParser::Dictionary.new
 *     LinkParser::Sentence.new( "The boy runs", dict )  #=> #<LinkParser::Sentence:0x5481ac>
 */
static VALUE
rlink_sentence_init( self, input_string, dictionary )
	VALUE self, input_string, dictionary;
{
	if ( !check_sentence(self) ) {
		rlink_SENTENCE *ptr;
		Sentence sent;
		Dictionary dict = rlink_get_dict( dictionary );
	
		if ( !(sent = sentence_create( StringValueCStr(input_string), dict )) )
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
 *     sentence.parse( options={} )   => fixnum
 *
 *  Attach a parse set to this sentence and return the number of linkages
 *  found. If any +options+ are specified, they override those set in the 
 *  sentence's dictionary.
 * 
 */
static VALUE
rlink_sentence_parse( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	Parse_Options opts;
	VALUE defopts = Qnil;
	VALUE options = Qnil;
	int link_count = 0;

	if ( RTEST(ptr->parsed_p) )
		rb_raise( rlink_eLpError, "Can't reparse a sentence." );

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
 *     sentence.parsed?   => true or false
 *
 *  Returns +true+ if the sentence has been parsed.
 *
 *     sentence.parsed?   #=> false
 *     sentence.parse     #=> 6
 *     sentence.parsed?   #=> true
 */
static VALUE
rlink_sentence_parsed_p( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	return ptr->parsed_p;
}


/*
 *  call-seq:
 *     sentence.linkages   => array
 *
 *  Returns an Array of LinkParser::Linkage objects which represent the
 *  parts parsed from the sentence for the current linkage.
 *
 */
static VALUE
rlink_sentence_linkages( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int i, count = 0;
	VALUE rary;

	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );

	count = sentence_num_linkages_found( (Sentence)ptr->sentence );
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
 *     sentence.length   => fixnum
 *
 *  Returns the number of words in the tokenized sentence, including the 
 *  boundary words and punctuation.
 *
 */

static VALUE
rlink_sentence_length( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	return INT2FIX( sentence_length((Sentence)ptr->sentence) );
}
	

/*
 *  call-seq:
 *     sentence.word( idx )   => str
 *
 *  Returns the spelling of the n-th word in the sentence as it appears after 
 *  tokenization.
 */
static VALUE
rlink_sentence_word( self, n )
	VALUE self, n;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	char *word;
	
	word = sentence_get_word( (Sentence)ptr->sentence, FIX2INT(n) );
	return rb_str_new2( word );
}


/*
 *  call-seq:
 *     sentence.words   => array
 *
 *  Returns the words of the sentence as they appear after tokenization.
 *
 *     sentence = LinkParser::Dictionary.new.parse( "The dogs barks." )
 *     sentence.words  #=> 
 */
static VALUE
rlink_sentence_words( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	char *word;
	int i, length;
	VALUE words = rb_ary_new();
	
	length = sentence_length( (Sentence)ptr->sentence );
	for ( i = 0; i < length; i++ ) {
		word = sentence_get_word( (Sentence)ptr->sentence, i );
		rb_ary_push( words, rb_str_new2(word) );
	}
	
	return words;
}


/*
 *  call-seq:
 *     sentence[index]   		 => str
 *     sentence[start, length]   => str
 *     sentence[range]   		 => str
 *
 *  Element Reference---Returns the element at index, or returns a subarray 
 *  starting at start and continuing for length elements, or returns a subarray 
 *  specified by range. Negative indices count backward from the end of the 
 *  array (-1 is the last element). Returns nil if the index (or starting 
 *  index) are out of range.
 *
 *     sent = dict.parse( "Birds fly south for the winter." )
 *     
 *     sent[1]		# => "birds"
 *     sent[0,4]	# => ["LEFT-WALL", "birds", "fly", "south"]
 *     sent[1..3]	# => ["birds", "fly", "south"]
 *     
 */
static VALUE
rlink_sentence_aref( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE words = rlink_sentence_words( self );
	return rb_funcall2( words, rb_intern("[]"), argc, argv );
}


/*
 *  call-seq:
 *     sentence.null_count   => int
 *
 *  Returns the number of null links that were used in parsing the sentence.
 */
static VALUE
rlink_sentence_null_count( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int count;
	
	count = sentence_null_count( (Sentence)ptr->sentence );
	return INT2FIX( count );
}
	

/*
 *  call-seq:
 *     sentence.num_linkages_found   => fixnum
 *
 *  Returns the number of linkages found when parsing the sentence. This will 
 *  cause the sentence to be parsed if it hasn't been already.
 */
static VALUE
rlink_sentence_num_linkages_found( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int i = 0;
	
	if ( !RTEST(ptr->parsed_p) )
		rlink_sentence_parse( 0, 0, self );
	i = sentence_num_linkages_found( (Sentence)ptr->sentence );
	
	return INT2FIX( i );
}


/*
 *  call-seq:
 *     sentence.num_valid_linkages   => fixnum
 *
 *  Return the number of linkages that had no post-processing violations.
 */
static VALUE
rlink_sentence_num_valid_linkages( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int count;
	
	count = sentence_num_valid_linkages( (Sentence)ptr->sentence );
	return INT2FIX( count );
}
	

/*
 *  call-seq:
 *     sentence.num_linkages_post_processed   => fixnum
 *
 *  Return the number of linkages that were actually post-processed (which may 
 *  be less than the number found because of the linkage_limit parameter).
 */
static VALUE
rlink_sentence_num_linkages_post_processed( self )
	VALUE self;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int count;
	
	count = sentence_num_linkages_post_processed( (Sentence)ptr->sentence );
	return INT2FIX( count );
}
	

/*
 *  call-seq:
 *     sentence.num_violations( i )   => fixnum
 *
 *  The number of post-processing violations that the i-th linkage had during 
 *  the last parse.
 */
static VALUE
rlink_sentence_num_violations( self, i )
	VALUE self, i;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int count;
	
	count = sentence_num_violations( (Sentence)ptr->sentence, FIX2INT(i) );
	return INT2FIX( count );
}
	

/*
 *  call-seq:
 *     sentence.disjunct_cost( i )   => fixnum
 *
 *  The maximum cost of connectors used in the i-th linkage of the sentence.
 */
static VALUE
rlink_sentence_disjunct_cost( self, i )
	VALUE self, i;
{
	rlink_SENTENCE *ptr = get_sentence( self );
	int count;
	
	count = sentence_disjunct_cost( (Sentence)ptr->sentence, FIX2INT(i) );
	return INT2FIX( count );
}
	

/* Class Initializer */
void
rlink_init_sentence( void )
{
#ifdef FOR_RDOC
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );
#endif

	rlink_cSentence = rb_define_class_under( rlink_mLinkParser, "Sentence",
	 	rb_cObject );
	
	rb_define_alloc_func( rlink_cSentence, rlink_sentence_s_alloc );

	rb_define_method( rlink_cSentence, "initialize", rlink_sentence_init, 2 );
	rb_define_method( rlink_cSentence, "parse", rlink_sentence_parse, -1 );
	rb_define_method( rlink_cSentence, "parsed?", rlink_sentence_parsed_p, 0 );
	rb_define_method( rlink_cSentence, "linkages", rlink_sentence_linkages, 0 );

	rb_define_method( rlink_cSentence, "length", rlink_sentence_length, 0 );
	rb_define_method( rlink_cSentence, "word", rlink_sentence_word, 1 );
	rb_define_method( rlink_cSentence, "words", rlink_sentence_words, 0 );
	rb_define_method( rlink_cSentence, "[]", rlink_sentence_aref, -1 );

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

/*
	link_public_api(char *) sentence_get_nth_word(Sentence sent, int i);
	link_public_api(int) sentence_nth_word_has_disjunction(Sentence sent, int i);
*/

}

