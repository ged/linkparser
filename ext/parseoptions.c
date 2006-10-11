/*
 *  parseoptions.c - Ruby LinkParser::ParseOptions class
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
rlink_parseopts_gc_free( parseopts )
	Parse_Options parseopts;
{
	if ( parseopts ) parse_options_delete( parseopts );
}


/*
 * Object validity checker. Returns the data pointer.
 */
static Parse_Options
check_parseopts( self )
	 VALUE	self;
{
	debugMsg(( "Checking a LinkParser::ParseOptions object (%d).", self ));
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
get_parseopts( self )
	 VALUE self;
{
	Parse_Options parseopts = check_parseopts( self );

	debugMsg(( "Fetching a Parse_Options (%p).", parseopts ));
	if ( !parseopts )
		rb_raise( rb_eRuntimeError, "uninitialized ParseOptions" );

	return parseopts;
}


/* 
 * Get the Parse_Options struct behind the LinkParser::ParseOptions +object+ 
 * specified.
 */ 
Parse_Options
rlink_get_parseopts( obj )
	VALUE obj;
{
	return get_parseopts( obj );
}


/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 * allocate()
 * --
 * Allocate a new LinkParser::ParseOptions object.
 */
static VALUE
rlink_parseopts_s_alloc( klass )
	 VALUE klass;
{
	debugMsg(( "Wrapping an uninitialized ParseOptions pointer." ));
	return Data_Wrap_Struct( klass, 0, rlink_parseopts_gc_free, 0 );
}



/* ---------------------------------------------------
 * Instance Methods
 * --------------------------------------------------- */


/*
 * initialize( opthash={} )
 * --
 * Create a new ParseOptions object and set values from opthash.
 */
static VALUE
rlink_parseopts_init( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
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


static VALUE
rlink_parseopts_each_opthash_i( pair, self )
	VALUE pair, self;
{
	VALUE key, val, keystring;
	char *method_name;
	ID method;
	
	key = rb_ary_entry( pair, 0 );
	val = rb_ary_entry( pair, 1 );

	keystring = rb_obj_as_string( key );
	
	method_name = ALLOCA_N( char, RSTRING(keystring)->len + 1 );
	strncpy( method_name, RSTRING(keystring)->ptr, RSTRING(keystring)->len + 1 );
	strncat( method_name, "=", 1 );
	
	debugMsg(( "Calling method %s", method_name ));
	method = rb_intern( method_name );
	
	return rb_funcall( self, method, 1, val );
}


/*
 * merge( other )
 * --
 * Merge the receiving parse options with the given +other+ object, which can
 * be either another LinkParser::ParseOptions object or a Hash of options.
 */
/*static VALUE
rlink_parseopts_merge( self, other )
	VALUE self, other;
{
	
}
*/


/*
 * verbosity=( int )
 * --
 * This sets the level of description printed to stderr/stdout about the 
 * parsing process.
 */
static VALUE
rlink_parseopts_set_verbosity( self, verbosity )
	VALUE self, verbosity;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_verbosity( opts, NUM2INT(verbosity) );
	return verbosity;
}

/*
 * verbosity => int
 * --
 * This gets the level of description printed to stderr/stdout about the 
 * parsing process.
 */
static VALUE
rlink_parseopts_get_verbosity( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_verbosity( opts );
	return INT2FIX( rval );
}

/*
 * linkage_limit=( int )
 * --
 * This parameter determines the maximum number of linkages that are 
 * considered in post-processing. If more than +linkage_limit+ linkages are found, 
 * then a random sample of +linkage_limit+ is chosen for post-processing. When 
 * this happen a warning is displayed at verbosity levels greater than 1.
 */
static VALUE
rlink_parseopts_set_linkage_limit( self, linkage_limit )
	VALUE self, linkage_limit;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_linkage_limit( opts, NUM2INT(linkage_limit) );
	return linkage_limit;
}

/*
 * linkage_limit => int
 * --
 * This parameter determines the maximum number of linkages that are 
 * considered in post-processing. If more than +linkage_limit+ linkages are found, 
 * then a random sample of +linkage_limit+ is chosen for post-processing. When 
 * this happen a warning is displayed at verbosity levels greater than 1.
 */
static VALUE
rlink_parseopts_get_linkage_limit( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_linkage_limit( opts );
	return INT2FIX( rval );
}

/*
 * disjunct_cost=( int )
 * --
 * Determines the maximum disjunct cost used during parsing, where the cost 
 * of a disjunct is equal to the maximum cost of all of its connectors. The 
 * default is that all disjuncts, no matter what their cost, are considered.
 */
static VALUE
rlink_parseopts_set_disjunct_cost( self, disjunct_cost )
	VALUE self, disjunct_cost;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_disjunct_cost( opts, NUM2INT(disjunct_cost) );
	return disjunct_cost;
}

/*
 * disjunct_cost => int
 * --
 * Get the maximum disjunct cost used during parsing.
 */
static VALUE
rlink_parseopts_get_disjunct_cost( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_disjunct_cost( opts );
	return INT2FIX( rval );
}

/*
 * min_null_count=( null_count )
 * --
 * Set the minimum of null links that a parse can have. A call to 
 * LinkParser::Sentence#parse will find all linkages having the minimum 
 * number of null links within the range specified by this parameter.
 */
static VALUE
rlink_parseopts_set_min_null_count( self, null_count )
	VALUE self, null_count;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_min_null_count( opts, NUM2INT(null_count) );
	return null_count;
}

/*
 * min_null_count => fixnum
 * --
 * Get the minimum of null links that a parse can have.
 */
static VALUE
rlink_parseopts_get_min_null_count( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_min_null_count( opts );
	return INT2FIX( rval );
}

/*
 * max_null_count=( null_count )
 * --
 * Set the max_null_count option to the specified value.
 */
static VALUE
rlink_parseopts_set_max_null_count( self, null_count )
	VALUE self, null_count;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_null_count( opts, NUM2INT(null_count) );
	return null_count;
}

/*
 * max_null_count
 * --
 * Get the value of the max_null_count option.
 */
static VALUE
rlink_parseopts_get_max_null_count( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_null_count( opts );
	return INT2FIX( rval );
}

/*
 * null_block=( null_block )
 * --
 * Set the null_block option to the specified value.
 */
static VALUE
rlink_parseopts_set_null_block( self, null_block )
	VALUE self, null_block;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_null_block( opts, NUM2INT(null_block) );
	return null_block;
}

/*
 * null_block
 * --
 * Get the value of the null_block option.
 */
static VALUE
rlink_parseopts_get_null_block( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_null_block( opts );
	return INT2FIX( rval );
}

/*
 * islands_ok=( islands_ok )
 * --
 * Set the islands_ok option to the specified value.
 */
static VALUE
rlink_parseopts_set_islands_ok( self, islands_ok )
	VALUE self, islands_ok;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_islands_ok( opts, RTEST(islands_ok) );
	return islands_ok;
}

/*
 * islands_ok
 * --
 * Get the value of the islands_ok option.
 */
static VALUE
rlink_parseopts_get_islands_ok_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_islands_ok( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * short_length=( short_length )
 * --
 * Set the short_length option to the specified value.
 */
static VALUE
rlink_parseopts_set_short_length( self, short_length )
	VALUE self, short_length;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_short_length( opts, NUM2INT(short_length) );
	return short_length;
}

/*
 * short_length
 * --
 * Get the value of the short_length option.
 */
static VALUE
rlink_parseopts_get_short_length( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_short_length( opts );
	return INT2FIX( rval );
}

/*
 * max_memory=( mem )
 * --
 * Set the max_memory option to the specified value.
 */
static VALUE
rlink_parseopts_set_max_memory( self, mem )
	VALUE self, mem;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_memory( opts, NUM2INT(mem) );
	return mem;
}

/*
 * max_memory
 * --
 * Get the value of the max_memory option.
 */
static VALUE
rlink_parseopts_get_max_memory( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_memory( opts );
	return INT2FIX( rval );
}

/*
 * max_sentence_length=( len )
 * --
 * Set the max_sentence_length option to the specified value.
 */
static VALUE
rlink_parseopts_set_max_sentence_length( self, len )
	VALUE self, len;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_sentence_length( opts, NUM2INT(len) );
	return len;
}

/*
 * max_sentence_length
 * --
 * Get the value of the max_sentence_length option.
 */
static VALUE
rlink_parseopts_get_max_sentence_length( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_sentence_length( opts );
	return INT2FIX( rval );
}

/*
 * max_parse_time=( secs )
 * --
 * Set the max_parse_time option to the specified value.
 */
static VALUE
rlink_parseopts_set_max_parse_time( self, secs )
	VALUE self, secs;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_max_parse_time( opts, NUM2INT(secs) );
	return secs;
}

/*
 * max_parse_time
 * --
 * Get the value of the max_parse_time option.
 */
static VALUE
rlink_parseopts_get_max_parse_time( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_parse_time( opts );
	return INT2FIX( rval );
}

/*
 * screen_width=( val )
 * --
 * Set the screen_width option to the specified value.
 */
static VALUE
rlink_parseopts_set_screen_width( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_screen_width( opts, NUM2INT(val) );
	return val;
}

/*
 * screen_width
 * --
 * Get the value of the screen_width option.
 */
static VALUE
rlink_parseopts_get_screen_width( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_screen_width( opts );
	return INT2FIX( rval );
}

/*
 * allow_null=( val )
 * --
 * Set the allow_null option to the specified value.
 */
static VALUE
rlink_parseopts_set_allow_null( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_allow_null( opts, RTEST(val) );
	return val;
}

/*
 * allow_null?
 * --
 * Get the value of the allow_null option.
 */
static VALUE
rlink_parseopts_get_allow_null_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_allow_null( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_walls=( val )
 * --
 * Set the display_walls option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_walls( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_walls( opts, RTEST(val) );
	return val;
}

/*
 * display_walls
 * --
 * Get the value of the display_walls option.
 */
static VALUE
rlink_parseopts_get_display_walls_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_walls( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * all_short_connectors=( val )
 * --
 * Set the all_short_connectors option to the specified value.
 */
static VALUE
rlink_parseopts_set_all_short_connectors( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_all_short_connectors( opts, RTEST(val) );
	return val;
}

/*
 * all_short_connectors
 * --
 * Get the value of the all_short_connectors option.
 */
static VALUE
rlink_parseopts_get_all_short_connectors_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_all_short_connectors( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * cost_model_type=( cm )
 * --
 * Set the cost_model_type option to the specified value.
 */
static VALUE
rlink_parseopts_set_cost_model_type( self, cm )
	VALUE self, cm;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_cost_model_type( opts, NUM2INT(cm) );
	return cm;
}

/*
 * cost_model_type
 * --
 * Get the value of the cost_model_type option.
 */
/*

There's no actual API function for getting the cost_model_type. I guess if
there's ever more than one model type defined there will be.

static VALUE
rlink_parseopts_get_cost_model_type( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_cost_model_type( opts );
	return INT2FIX( rval );
}
*/

/*
 * batch_mode=( val )
 * --
 * Set the batch_mode option to the specified value.
 */
static VALUE
rlink_parseopts_set_batch_mode( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_batch_mode( opts, RTEST(val) );
	return val;
}

/*
 * batch_mode
 * --
 * Get the value of the batch_mode option.
 */
static VALUE
rlink_parseopts_get_batch_mode_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_batch_mode( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * panic_mode=( val )
 * --
 * Set the panic_mode option to the specified value.
 */
static VALUE
rlink_parseopts_set_panic_mode( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_panic_mode( opts, RTEST(val) );
	return val;
}

/*
 * panic_mode
 * --
 * Get the value of the panic_mode option.
 */
static VALUE
rlink_parseopts_get_panic_mode_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_panic_mode( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_on=( val )
 * --
 * Set the display_on option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_on( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_on( opts, RTEST(val) );
	return val;
}

/*
 * display_on
 * --
 * Get the value of the display_on option.
 */
static VALUE
rlink_parseopts_get_display_on_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_on( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_postscript=( val )
 * --
 * Set the display_postscript option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_postscript( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_postscript( opts, RTEST(val) );
	return val;
}

/*
 * display_postscript
 * --
 * Get the value of the display_postscript option.
 */
static VALUE
rlink_parseopts_get_display_postscript_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_postscript( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_constituents=( val )
 * --
 * Set the display_constituents option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_constituents( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_constituents( opts, RTEST(val) );
	return val;
}

/*
 * display_constituents
 * --
 * Get the value of the display_constituents option.
 */
static VALUE
rlink_parseopts_get_display_constituents_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_constituents( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_bad=( val )
 * --
 * Set the display_bad option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_bad( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_bad( opts, RTEST(val) );
	return val;
}

/*
 * display_bad
 * --
 * Get the value of the display_bad option.
 */
static VALUE
rlink_parseopts_get_display_bad_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_bad( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_links=( val )
 * --
 * Set the display_links option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_links( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_links( opts, RTEST(val) );
	return val;
}

/*
 * display_links
 * --
 * Get the value of the display_links option.
 */
static VALUE
rlink_parseopts_get_display_links_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_links( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * display_union=( val )
 * --
 * Set the display_union option to the specified value.
 */
static VALUE
rlink_parseopts_set_display_union( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_display_union( opts, RTEST(val) );
	return val;
}

/*
 * display_union
 * --
 * Get the value of the display_union option.
 */
static VALUE
rlink_parseopts_get_display_union_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_display_union( opts );
	return rval ? Qtrue : Qfalse;
}

/*
 * echo_on=( val )
 * --
 * Set the echo_on option to the specified value.
 */
static VALUE
rlink_parseopts_set_echo_on( self, val )
	VALUE self, val;
{
	Parse_Options opts = get_parseopts( self );
	parse_options_set_echo_on( opts, RTEST(val) );
	return val;
}

/*
 * echo_on
 * --
 * Get the value of the echo_on option.
 */
static VALUE
rlink_parseopts_get_echo_on_p( self )
	VALUE self;
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_echo_on( opts );
	return rval ? Qtrue : Qfalse;
}


/*
rlink_parseopts_timer_expired(Parse_Options opts);
rlink_parseopts_memory_exhausted(Parse_Options opts);
rlink_parseopts_resources_exhausted(Parse_Options opts);
rlink_parseopts_reset_resources(Parse_Options opts);
*/

/* --------------------------------------------------------------
 * Extension Initializer
 * -------------------------------------------------------------- */
void
rlink_init_parseoptions(void)
{
#ifdef FOR_RDOC
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );
#endif
	
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

/*
	rlink_parseopts_timer_expired
	rlink_parseopts_memory_exhausted
	rlink_parseopts_resources_exhausted
	rlink_parseopts_reset_resources
*/
}

