/*
 *  parseoptions.c - Ruby LinkParser::ParseOptions class
 *  $Id: parseoptions.c,v 21df914cedb1 2010/10/12 15:45:26 ged $
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

static VALUE rlink_parseopts_each_opthash_i _(( VALUE, VALUE ));


/* --------------------------------------------------
 * Macros and constants
 * -------------------------------------------------- */


/* --------------------------------------------------
 *	Memory-management functions
 * -------------------------------------------------- */

/*
 * Free function
 */
static void
rlink_parseopts_gc_free( Parse_Options parseopts ) {
	if ( parseopts ) parse_options_delete( parseopts );
}


/*
 * Object validity checker. Returns the data pointer.
 */
static Parse_Options
check_parseopts( VALUE self ) {
	Check_Type( self, T_DATA );

    if ( !IsParseOptions(self) ) {
		rb_raise( rb_eTypeError, "wrong argument type %s (expected LinkParser::ParseOptions)",
				  rb_class2name(CLASS_OF( self )) );
    }

	return DATA_PTR( self );
}


/*
 * Fetch the data pointer and check it for sanity.
 */
static Parse_Options
get_parseopts( VALUE self ) {
	Parse_Options parseopts = check_parseopts( self );

	if ( !parseopts )
		rb_raise( rb_eRuntimeError, "uninitialized ParseOptions" );

	return parseopts;
}


/* 
 * Get the Parse_Options struct behind the LinkParser::ParseOptions +object+ 
 * specified.
 */ 
Parse_Options
rlink_get_parseopts( VALUE obj ) {
	return get_parseopts( obj );
}


/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 *  call-seq:
 *     LinkParser::ParseOptions.allocate   -> obj
 *
 *  Allocate a new LinkParser::ParseOptions object.
 */
static VALUE
rlink_parseopts_s_alloc( VALUE klass ) {
	debugMsg(( "Wrapping an uninitialized ParseOptions pointer." ));
	return Data_Wrap_Struct( klass, 0, rlink_parseopts_gc_free, 0 );
}



/* ---------------------------------------------------
 * Instance Methods
 * --------------------------------------------------- */


/*
 *  call-seq:
 *     LinkParser::ParseOptions.new( opthash )   -> obj
 *
 *  Create a new ParseOptions object and set values from opthash.
 *  
 *     po = LinkParser::ParseOptions.new( :allow_null => true, :batch_mode => true )
 *     
 */
static VALUE
rlink_parseopts_init( int argc, VALUE *argv, VALUE self ) {
	if ( ! check_parseopts(self) ) {
		Parse_Options opts;
		VALUE opthash = Qnil;

		debugMsg(( "Initializing a ParseOptions: %p", self ));
		DATA_PTR( self ) = opts = parse_options_create();

		rb_scan_args( argc, argv, "01", &opthash );
		if ( RTEST(opthash) ) {
			debugMsg(( "Setting options from an opthash." ));
			rb_iterate( rb_each, opthash, rlink_parseopts_each_opthash_i, self );
		}
	}

	else {
		rb_raise( rb_eRuntimeError, "Cannot re-initialize a Dictionary object." );
	}

	return self;
}


/* 
 *  Iterator function for rlink_parseopts_init() -- for each element of the hash passed
 *  to the constructor, call the corresponding accessor in the new object.
 */
static VALUE
rlink_parseopts_each_opthash_i( VALUE pair, VALUE self ) {
	VALUE key, val, keystring;
	char *method_name;
	ID method;

	key = rb_ary_entry( pair, 0 );
	val = rb_ary_entry( pair, 1 );

	keystring = rb_obj_as_string( key );

	method_name = ALLOCA_N( char, RSTRING_LEN(keystring) + 1 );
	strncpy( method_name, RSTRING_PTR(keystring), RSTRING_LEN(keystring) + 1 );
	strncat( method_name, "=", 1 );

	debugMsg(( "Calling method %s", method_name ));
	method = rb_intern( method_name );

	return rb_funcall( self, method, 1, val );
}


/*
 *  call-seq:
 *     merge( other )   -> parseopts
 *
 *  Merge the receiving parse options with the given +other+ object, which can
 *  be either another LinkParser::ParseOptions object or a Hash of options.
 */
/*static VALUE
rlink_parseopts_merge( VALUE self, other ) {

}
*/


/*
 *  call-seq:
 *     opts.verbosity= fixnum
 *
 *  This sets the level of description printed to stderr/stdout about the 
 *  parsing process.
 */
static VALUE
rlink_parseopts_set_verbosity( VALUE self, VALUE verbosity ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_verbosity( opts, NUM2INT(verbosity) );
	return verbosity;
}


/*
 *  call-seq:
 *     opts.verbosity   -> fixnum
 *
 *  This gets the level of description printed to stderr/stdout about the 
 *  parsing process.
 */
static VALUE
rlink_parseopts_get_verbosity( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_verbosity( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.linkage_limit= fixnum
 *
 *  This parameter determines the maximum number of linkages that are 
 *  considered in post-processing. If more than +linkage_limit+ linkages are found, 
 *  then a random sample of +linkage_limit+ is chosen for post-processing. When 
 *  this happen a warning is displayed at verbosity levels greater than 1.
 */
static VALUE
rlink_parseopts_set_linkage_limit( VALUE self, VALUE linkage_limit ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_linkage_limit( opts, NUM2INT(linkage_limit) );
	return linkage_limit;
}


/*
 *  call-seq:
 *     opts.linkage_limit   -> fixnum
 *
 *  This parameter determines the maximum number of linkages that are 
 *  considered in post-processing. If more than +linkage_limit+ linkages are found, 
 *  then a random sample of +linkage_limit+ is chosen for post-processing. When 
 *  this happen a warning is displayed at verbosity levels greater than 1.
 */
static VALUE
rlink_parseopts_get_linkage_limit( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_linkage_limit( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.disjunct_cost= fixnum
 *
 *  Determines the maximum disjunct cost used during parsing, where the cost 
 * of a disjunct is equal to the maximum cost of all of its connectors. The 
 * default is that all disjuncts, no matter what their cost, are considered.
 */
static VALUE
rlink_parseopts_set_disjunct_cost( VALUE self, VALUE disjunct_cost ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_disjunct_cost( opts, NUM2INT(disjunct_cost) );
	return disjunct_cost;
}


/*
 *  call-seq:
 *     opts.disjunct_cost   -> fixnum
 *
 *  Get the maximum disjunct cost used during parsing.
 */
static VALUE
rlink_parseopts_get_disjunct_cost( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_disjunct_cost( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.min_null_count= fixnum   -> fixnum
 *
 *  Set the minimum of null links that a parse can have. A call to 
 *  LinkParser::Sentence#parse will find all linkages having the minimum 
 *  number of null links within the range specified by this parameter.
 */
static VALUE
rlink_parseopts_set_min_null_count( VALUE self, VALUE null_count ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_min_null_count( opts, NUM2INT(null_count) );
	return null_count;
}


/*
 *  call-seq:
 *     opts.min_null_count   -> fixnum
 *
 *  Get the minimum of null links that a parse can have.
 */
static VALUE
rlink_parseopts_get_min_null_count( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_min_null_count( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.max_null_count= fixnum
 *
 *  Set the maximum number of null links allowed in a parse.
 */
static VALUE
rlink_parseopts_set_max_null_count( VALUE self, VALUE null_count ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_null_count( opts, NUM2INT(null_count) );
	return null_count;
}


/*
 *  call-seq:
 *     opts.max_null_count   -> fixnum
 *
 *  Get the maximum number of null links allowed in a parse.
 */
static VALUE
rlink_parseopts_get_max_null_count( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_null_count( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.null_block= null_block
 *
 *  Set the null_block option to the specified value. The null_block option 
 *  allows null links to be counted in "bunches." For example, if null_block 
 *  is 4, then a linkage with 1,2,3 or 4 null links has a null cost of 1, a 
 *  linkage with 5,6,7 or 8 null links has a null cost of 2, etc.
 */
static VALUE
rlink_parseopts_set_null_block( VALUE self, VALUE null_block ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_null_block( opts, NUM2INT(null_block) );
	return null_block;
}


/*
 *  call-seq:
 *     opts.null_block   -> fixnum
 *
 *  Get the value of the null_block option.
 */
static VALUE
rlink_parseopts_get_null_block( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_null_block( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.islands_ok= boolean
 *
 *  This option determines whether or not "islands" of links are allowed. For 
 *  example, the following linkage has an island:
 *
 *       +------Wd-----+                                           
 *       |     +--Dsu--+---Ss--+-Paf-+      +--Dsu--+---Ss--+--Pa-+
 *       |     |       |       |     |      |       |       |     |
 *     ///// this sentence.n is.v false.a this sentence.n is.v true.a
 */
static VALUE
rlink_parseopts_set_islands_ok( VALUE self, VALUE islands_ok ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_islands_ok( opts, RTEST(islands_ok) );
	return islands_ok;
}


/*
 *  call-seq:
 *     opts.islands_ok?   -> true or false
 *
 *  Get the value of the islands_ok option.
 */
static VALUE
rlink_parseopts_get_islands_ok_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_islands_ok( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.short_length= fixnum
 *
 *  The short_length parameter determines how long the links are allowed to 
 *  be. The intended use of this is to speed up parsing by not considering 
 *  very long links for most connectors, since they are very rarely used in a 
 *  correct parse. An entry for UNLIMITED-CONNECTORS in the dictionary will 
 *  specify which connectors are exempt from the length limit.
 */
static VALUE
rlink_parseopts_set_short_length( VALUE self, VALUE short_length ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_short_length( opts, NUM2INT(short_length) );
	return short_length;
}


/*
 *  call-seq:
 *     opts.short_length   -> fixnum
 *
 *  Get the value of the short_length option.
 */
static VALUE
rlink_parseopts_get_short_length( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_short_length( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.max_memory= fixnum
 *
 *  Determines the maximum memory allowed during parsing. This is used just as 
 *  max_parse_time is, so that the parsing process is terminated as quickly as 
 *  possible after the total memory (including that allocated to all 
 *  dictionaries, etc.) exceeds the maximum allowed.
 */
static VALUE
rlink_parseopts_set_max_memory( VALUE self, VALUE mem ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_memory( opts, NUM2INT(mem) );
	return mem;
}


/*
 *  call-seq:
 *     opts.max_memory   -> fixnum
 *
 *  Get the value of the max_memory option.
 */
static VALUE
rlink_parseopts_get_max_memory( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_memory( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.max_sentence_length= fixnum
 *
 *  Determines the maximum length of a parsed sentence.
 */
static VALUE
rlink_parseopts_set_max_sentence_length( VALUE self, VALUE len ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_sentence_length( opts, NUM2INT(len) );
	return len;
}


/*
 *  call-seq:
 *     opts.max_sentence_length   -> fixnum
 *
 *  Get the value of the max_sentence_length option.
 */
static VALUE
rlink_parseopts_get_max_sentence_length( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_sentence_length( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.max_parse_time= seconds
 *
 *  Determines the approximate maximum time that parsing is allowed to take. 
 *  The way it works is that after this time has expired, the parsing process 
 *  is artificially forced to complete quickly by pretending that no further 
 *  solutions (entries in the hash table) can be constructed. The actual 
 *  parsing time might be slightly longer.
 */
static VALUE
rlink_parseopts_set_max_parse_time( VALUE self, VALUE secs ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_parse_time( opts, NUM2INT(secs) );
	return secs;
}


/*
 *  call-seq:
 *     opts.max_parse_time   -> fixnum
 *
 *  Get the number of seconds of the max_parse_time option.
 */
static VALUE
rlink_parseopts_get_max_parse_time( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_parse_time( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.screen_width= columns
 *
 *  Set the screen width assumed by the diagramming functions.
 */
static VALUE
rlink_parseopts_set_screen_width( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_screen_width( opts, NUM2INT(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.screen_width   -> fixnum
 *
 *  Get the screen width assumed by the diagramming functions.
 */
static VALUE
rlink_parseopts_get_screen_width( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_screen_width( opts );
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     opts.allow_null= boolean
 *
 *  Indicates whether or not linkages are allowed to have null links.
 */
static VALUE
rlink_parseopts_set_allow_null( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_allow_null( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.allow_null?   -> true or false
 *
 *  Get the value of the allow_null option.
 */
static VALUE
rlink_parseopts_get_allow_null_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_allow_null( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_walls= boolean
 *
 *  Whether or not to show the wall word(s) when a linkage diagram is printed.
 */
static VALUE
rlink_parseopts_set_display_walls( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_walls( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_walls?   -> true or false
 *
 *  Whether or not to show the wall word(s) when a linkage diagram is printed.
 */
static VALUE
rlink_parseopts_get_display_walls_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_walls( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.all_short_connectors= boolean
 *
 *  If true, then all connectors have length restrictions imposed on them -- 
 *  they can be no farther than short_length apart. This is used when parsing 
 *  in "panic" mode, for example.
 */
static VALUE
rlink_parseopts_set_all_short_connectors( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_all_short_connectors( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.all_short_connectors?   -> true or false
 *
 *  Get the value of the all_short_connectors option.
 */
static VALUE
rlink_parseopts_get_all_short_connectors_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_all_short_connectors( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.cost_model_type= 
 *
 *  The cost model type for ranking linkages, which is an index into an array 
 *  of function pointers. The current code only has a single entry, but others 
 *  could easily be added.
 */
static VALUE
rlink_parseopts_set_cost_model_type( VALUE self, VALUE cm ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_cost_model_type( opts, NUM2INT(cm) );
	return cm;
}


/*
 *  call-seq:
 *     opts.cost_model_type   -> fixnum
 *
 *  Get the cost model type for ranking linkages.
 */
/*

There's no actual API function for getting the cost_model_type. I guess if
there's ever more than one model type defined there will be.

static VALUE
rlink_parseopts_get_cost_model_type( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_cost_model_type( opts );
	return INT2FIX( rval );
}
*/


/*
 *  call-seq:
 *     opts.batch_mode= boolean
 *
 *  Enable or disable "batch mode."
 *
 *  :TODO: Figure out what batch mode is.
 */
static VALUE
rlink_parseopts_set_batch_mode( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_batch_mode( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.batch_mode?   -> true or false
 *
 *  Returns +true+ if batch mode is enabled.
 */
static VALUE
rlink_parseopts_get_batch_mode_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_batch_mode( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 *  call-seq:
 *     opts.panic_mode= boolean
 *
 *  Enable or disable "panic mode."
 *  
 *  :TODO: Figure out what enabling this option does. I only know about panic
 *  mode in the parser -- does this allow/disallow the parser from entering it?
 */
static VALUE
rlink_parseopts_set_panic_mode( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_panic_mode( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.panic_mode?   -> true or false
 *
 *  Returns +true+ if panic mode is enabled.
 */
static VALUE
rlink_parseopts_get_panic_mode_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_panic_mode( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_on= boolean
 *
 *  Enable/disable display.
 *  
 *  :TODO: Figure out what this setting does.
 *  
 */
static VALUE
rlink_parseopts_set_display_on( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_on( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_on?   -> true or false
 *
 *  Returns +true+ if ...?
 */
static VALUE
rlink_parseopts_get_display_on_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_on( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_postscript= boolean
 *
 *  Enable/disable display using Postscript.
 */
static VALUE
rlink_parseopts_set_display_postscript( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_postscript( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_postscript?   -> true or false
 *
 *  Returns +true+ if display should use Postscript instead of plain text.
 */
static VALUE
rlink_parseopts_get_display_postscript_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_postscript( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_constituents= boolean
 *
 *  Set the display_constituents option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_constituents( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_constituents( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_constituents?   -> true or false
 *
 *  Get the value of the display_constituents option.
 */
static VALUE
rlink_parseopts_get_display_constituents_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_constituents( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_bad= boolean
 *
 *  Set the display_bad option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_bad( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_bad( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_bad?   -> true or false
 *
 *  Get the value of the display_bad option.
 */
static VALUE
rlink_parseopts_get_display_bad_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_bad( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_links= boolean
 *
 *  Set the display_links option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_links( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_links( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_links?   -> true or false
 *
 *  Get the value of the display_links option.
 */
static VALUE
rlink_parseopts_get_display_links_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_links( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.display_union= boolean
 *
 *  Set the display_union option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_union( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_union( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.display_union?   -> true or false
 *
 *  Get the value of the display_union option.
 */
static VALUE
rlink_parseopts_get_display_union_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_union( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.echo_on= boolean
 *
 *  Set the echo_on option to the specified value.
 */
static VALUE
rlink_parseopts_set_echo_on( VALUE self, VALUE val ) {
	Parse_Options opts = get_parseopts( self );
	parse_options_set_echo_on( opts, RTEST(val) );
	return val;
}


/*
 *  call-seq:
 *     opts.echo_on?   -> true or false
 *
 *  Get the value of the echo_on option.
 */
static VALUE
rlink_parseopts_get_echo_on_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_echo_on( opts );
	return rval ? Qtrue : Qfalse;
}




/*
 *  call-seq:
 *     opts.spell_guessing_enabled= boolean
 *
 *  Enable/disable spell-guessing if it's supported.
 */
static VALUE
rlink_parseopts_set_spell_guess( VALUE self, VALUE val ) {
#ifdef HAVE_PARSE_OPTIONS_GET_SPELL_GUESS
	Parse_Options opts = get_parseopts( self );
	parse_options_set_spell_guess( opts, RTEST(val) );
	return val;
#else
	rb_notimplement();
	return Qnil;
#endif /* HAVE_PARSE_OPTIONS_GET_SPELL_GUESS */
}


/*
 *  call-seq:
 *     opts.spell_guessing_enabled?   -> true or false
 *
 *  Returns +true+ if spell-guessing is enabled. Note that a +true+ return value doesn't
 *  mean that it's supported, only that it will be used if it is.
 */
static VALUE
rlink_parseopts_get_spell_guess_p( VALUE self ) {
#ifdef HAVE_PARSE_OPTIONS_GET_SPELL_GUESS
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_spell_guess( opts );
	return rval ? Qtrue : Qfalse;
#else
	rb_notimplement();
	return Qnil;
#endif /* HAVE_PARSE_OPTIONS_GET_SPELL_GUESS */
}


/*
 *  call-seq:
 *     opts.timer_expired?   -> +true+ or +false+
 *
 *  Returns true if timer constraints were exceeded during parsing.
 *
 *     sentence.parse
 *     if sentence.options.timer_expired?
 *       $stderr.puts "Parsing sentence #{sentence} timed out."
 *     end
 */
static VALUE
rlink_parseopts_timer_expired_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_timer_expired( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.memory_exhausted?   -> +true+ or +false+
 *
 *  Returns true if memory constraints were exceeded during parsing.
 *
 *     sentence.parse
 *     if sentence.options.memory_exhausted?
 *       $stderr.puts "Parsing sentence #{sentence} ran out of memory."
 *     end
 */
static VALUE
rlink_parseopts_memory_exhausted_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_memory_exhausted( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.resources_exhausted?   -> +true+ or +false+
 *
 *  Returns true if the memory or timer constraints were exceeded during parsing.
 *
 *     sentence.parse
 *     if sentence.options.resources_exhausted?
 *       $stderr.puts "Parsing sentence #{sentence} ran out of resources."
 *     end
 */
static VALUE
rlink_parseopts_resources_exhausted_p( VALUE self ) {
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_resources_exhausted( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.reset_resources
 *
 *  Reset the timer- and memory-constraint flags.
 *
 */
static VALUE
rlink_parseopts_reset_resources( VALUE self ) {
	Parse_Options opts = get_parseopts( self );

	parse_options_reset_resources( opts );
	return Qnil;
}



/*
 * LinkParser parse options class. Instances of this class are used to specify the different
 * parameters that are used to parse sentences. Examples of the kinds of things that are
 * controlled by ParseOptions include maximum parsing time and memory, whether to use
 * null-links, and whether or not to use "panic" mode. This data structure is passed in to
 * the various parsing and printing routines along with the sentence.
 *
 */
void
rlink_init_parseoptions() {
	rlink_cParseOptions = rb_define_class_under( rlink_mLinkParser,
		"ParseOptions", rb_cObject );

	rb_define_alloc_func( rlink_cParseOptions, rlink_parseopts_s_alloc );
	rb_define_method( rlink_cParseOptions, "initialize", rlink_parseopts_init, -1 );
/*
	rb_define_method( rlink_cParseOptions, "merge", rlink_parseopts_merge, 1 );
	rb_define_method( rlink_cParseOptions, "merge!", rlink_parseopts_merge_bang, 1 );
*/
	rb_define_method( rlink_cParseOptions, "verbosity=",
		rlink_parseopts_set_verbosity, 1 );
	rb_define_method( rlink_cParseOptions, "verbosity",
		rlink_parseopts_get_verbosity, 0 );
	rb_define_method( rlink_cParseOptions, "linkage_limit=",
		rlink_parseopts_set_linkage_limit, 1 );
	rb_define_method( rlink_cParseOptions, "linkage_limit",
		rlink_parseopts_get_linkage_limit, 0 );
	rb_define_method( rlink_cParseOptions, "disjunct_cost=",
		rlink_parseopts_set_disjunct_cost, 1 );
	rb_define_method( rlink_cParseOptions, "disjunct_cost",
		rlink_parseopts_get_disjunct_cost, 0 );
	rb_define_method( rlink_cParseOptions, "min_null_count=",
		rlink_parseopts_set_min_null_count, 1 );
	rb_define_method( rlink_cParseOptions, "min_null_count",
		rlink_parseopts_get_min_null_count, 0 );
	rb_define_method( rlink_cParseOptions, "max_null_count=",
		rlink_parseopts_set_max_null_count, 1 );
	rb_define_method( rlink_cParseOptions, "max_null_count",
		rlink_parseopts_get_max_null_count, 0 );
	rb_define_method( rlink_cParseOptions, "null_block=",
		rlink_parseopts_set_null_block, 1 );
	rb_define_method( rlink_cParseOptions, "null_block",
		rlink_parseopts_get_null_block, 0 );
	rb_define_method( rlink_cParseOptions, "islands_ok=",
		rlink_parseopts_set_islands_ok, 1 );
	rb_define_method( rlink_cParseOptions, "islands_ok?",
		rlink_parseopts_get_islands_ok_p, 0 );
	rb_define_method( rlink_cParseOptions, "short_length=",
		rlink_parseopts_set_short_length, 1 );
	rb_define_method( rlink_cParseOptions, "short_length",
		rlink_parseopts_get_short_length, 0 );
	rb_define_method( rlink_cParseOptions, "max_memory=",
		rlink_parseopts_set_max_memory, 1 );
	rb_define_method( rlink_cParseOptions, "max_memory",
		rlink_parseopts_get_max_memory, 0 );
	rb_define_method( rlink_cParseOptions, "max_sentence_length=",
		rlink_parseopts_set_max_sentence_length, 1 );
	rb_define_method( rlink_cParseOptions, "max_sentence_length",
		rlink_parseopts_get_max_sentence_length, 0 );
	rb_define_method( rlink_cParseOptions, "max_parse_time=",
		rlink_parseopts_set_max_parse_time, 1 );
	rb_define_method( rlink_cParseOptions, "max_parse_time",
		rlink_parseopts_get_max_parse_time, 0 );
	rb_define_method( rlink_cParseOptions, "screen_width=",
		rlink_parseopts_set_screen_width, 1 );
	rb_define_method( rlink_cParseOptions, "screen_width",
		rlink_parseopts_get_screen_width, 0 );
	rb_define_method( rlink_cParseOptions, "allow_null=",
		rlink_parseopts_set_allow_null, 1 );
	rb_define_method( rlink_cParseOptions, "allow_null?",
		rlink_parseopts_get_allow_null_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_walls=",
		rlink_parseopts_set_display_walls, 1 );
	rb_define_method( rlink_cParseOptions, "display_walls?",
		rlink_parseopts_get_display_walls_p, 0 );
	rb_define_method( rlink_cParseOptions, "all_short_connectors=",
		rlink_parseopts_set_all_short_connectors, 1 );
	rb_define_method( rlink_cParseOptions, "all_short_connectors?",
		rlink_parseopts_get_all_short_connectors_p, 0 );
	rb_define_method( rlink_cParseOptions, "cost_model_type=",
		rlink_parseopts_set_cost_model_type, 1 );

/*  (No way to get the cost_model_type from the API)

	rb_define_method( rlink_cParseOptions, "cost_model_type",
		rlink_parseopts_get_cost_model_type, 0 );
*/
	rb_define_method( rlink_cParseOptions, "batch_mode=",
		rlink_parseopts_set_batch_mode, 1 );
	rb_define_method( rlink_cParseOptions, "batch_mode?",
		rlink_parseopts_get_batch_mode_p, 0 );
	rb_define_method( rlink_cParseOptions, "panic_mode=",
		rlink_parseopts_set_panic_mode, 1 );
	rb_define_method( rlink_cParseOptions, "panic_mode?",
		rlink_parseopts_get_panic_mode_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_on=",
		rlink_parseopts_set_display_on, 1 );
	rb_define_method( rlink_cParseOptions, "display_on?",
		rlink_parseopts_get_display_on_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_postscript=",
		rlink_parseopts_set_display_postscript, 1 );
	rb_define_method( rlink_cParseOptions, "display_postscript?",
		rlink_parseopts_get_display_postscript_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_constituents=",
		rlink_parseopts_set_display_constituents, 1 );
	rb_define_method( rlink_cParseOptions, "display_constituents?",
		rlink_parseopts_get_display_constituents_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_bad=",
		rlink_parseopts_set_display_bad, 1 );
	rb_define_method( rlink_cParseOptions, "display_bad?",
		rlink_parseopts_get_display_bad_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_links=",
		rlink_parseopts_set_display_links, 1 );
	rb_define_method( rlink_cParseOptions, "display_links?",
		rlink_parseopts_get_display_links_p, 0 );
	rb_define_method( rlink_cParseOptions, "display_union=",
		rlink_parseopts_set_display_union, 1 );
	rb_define_method( rlink_cParseOptions, "display_union?",
		rlink_parseopts_get_display_union_p, 0 );
	rb_define_method( rlink_cParseOptions, "echo_on=",
		rlink_parseopts_set_echo_on, 1 );
	rb_define_method( rlink_cParseOptions, "echo_on?",
		rlink_parseopts_get_echo_on_p, 0 );
	rb_define_method( rlink_cParseOptions, "spell_guessing_enabled=",
		rlink_parseopts_set_spell_guess, 1 );
	rb_define_method( rlink_cParseOptions, "spell_guessing_enabled?",
		rlink_parseopts_get_spell_guess_p, 0 );

	rb_define_method( rlink_cParseOptions, "timer_expired?",
		rlink_parseopts_timer_expired_p, 0 );
	rb_define_method( rlink_cParseOptions, "memory_exhausted?",
		rlink_parseopts_memory_exhausted_p, 0 );
	rb_define_method( rlink_cParseOptions, "resources_exhausted?",
		rlink_parseopts_resources_exhausted_p, 0 );
	rb_define_method( rlink_cParseOptions, "reset_resources",
		rlink_parseopts_reset_resources, 0 );

}

