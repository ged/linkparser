/*
 *  parseoptions.c - Ruby LinkParser::ParseOptions class
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

VALUE vdal_sym;
VALUE corpus_sym;

/* --------------------------------------------------
 *	Memory-management functions
 * -------------------------------------------------- */

/*
 * Free function
 */
static void
rlink_parseopts_gc_free( Parse_Options parseopts )
{
	if ( parseopts ) {
		parse_options_delete( parseopts );
	}
}


/*
 * Object validity checker. Returns the data pointer.
 */
static Parse_Options
check_parseopts( VALUE self )
{
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
get_parseopts( VALUE self )
{
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
rlink_get_parseopts( VALUE obj )
{
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
rlink_parseopts_s_alloc( VALUE klass )
{
	rlink_log( "debug", "Wrapping an uninitialized ParseOptions pointer."  );
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
 *     po = LinkParser::ParseOptions.new( min_null_count: 1, verbosity: 0 )
 *
 */
static VALUE
rlink_parseopts_init( int argc, VALUE *argv, VALUE self )
{
	if ( ! check_parseopts(self) ) {
		Parse_Options opts;
		VALUE opthash = Qnil;

		rlink_log_obj( self, "debug", "Initializing a ParseOptions: %p", self );
		DATA_PTR( self ) = opts = parse_options_create();

		rb_scan_args( argc, argv, "01", &opthash );
		if ( RTEST(opthash) ) {
			rlink_log_obj( self, "debug", "Setting options from an opthash." );
			rb_funcall( self, rb_intern("merge!"), 1, opthash );
		}
	}

	else {
		rb_raise( rb_eRuntimeError, "Cannot re-initialize a ParseOptions object." );
	}

	return self;
}


/*
 * Copy constructor
 */
static VALUE
rlink_parseopts_init_copy( VALUE self, VALUE other )
{
	if ( ! check_parseopts(self) ) {
		Parse_Options opts;

		rlink_log_obj( self, "debug", "Initializing a copied ParseOptions: %p", self );
		DATA_PTR( self ) = opts = parse_options_create();
		rb_funcall( self, rb_intern("merge!"), 1, other );

		rb_call_super( 1, &other );
	}

	else {
		rb_raise( rb_eRuntimeError, "Can't recopy a ParseOptions object." );
	}

	return self;
}


/*
 *  call-seq:
 *     opts.verbosity= fixnum
 *
 *  This sets the level of description printed to stderr/stdout about the
 *  parsing process.
 */
static VALUE
rlink_parseopts_set_verbosity( VALUE self, VALUE verbosity )
{
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
rlink_parseopts_get_verbosity( VALUE self )
{
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
rlink_parseopts_set_linkage_limit( VALUE self, VALUE linkage_limit )
{
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
rlink_parseopts_get_linkage_limit( VALUE self )
{
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
 *  of a disjunct is equal to the maximum cost of all of its connectors. The
 *  default is that all disjuncts, no matter what their cost, are considered.
 */
static VALUE
rlink_parseopts_set_disjunct_cost( VALUE self, VALUE disjunct_cost )
{
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
rlink_parseopts_get_disjunct_cost( VALUE self )
{
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
rlink_parseopts_set_min_null_count( VALUE self, VALUE null_count )
{
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
rlink_parseopts_get_min_null_count( VALUE self )
{
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
rlink_parseopts_set_max_null_count( VALUE self, VALUE null_count )
{
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
rlink_parseopts_get_max_null_count( VALUE self )
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_null_count( opts );
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
rlink_parseopts_set_islands_ok( VALUE self, VALUE islands_ok )
{
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
rlink_parseopts_get_islands_ok_p( VALUE self )
{
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
rlink_parseopts_set_short_length( VALUE self, VALUE short_length )
{
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
rlink_parseopts_get_short_length( VALUE self )
{
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
rlink_parseopts_set_max_memory( VALUE self, VALUE mem )
{
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
rlink_parseopts_get_max_memory( VALUE self )
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_memory( opts );
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
rlink_parseopts_set_max_parse_time( VALUE self, VALUE secs )
{
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
rlink_parseopts_get_max_parse_time( VALUE self )
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_max_parse_time( opts );
	return INT2FIX( rval );
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
rlink_parseopts_set_all_short_connectors( VALUE self, VALUE val )
{
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
rlink_parseopts_get_all_short_connectors_p( VALUE self )
{
	Parse_Options opts = get_parseopts( self );
	int rval;

	rval = parse_options_get_all_short_connectors( opts );
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     opts.cost_model_type = :vdal
 *     opts.cost_model_type = :corpus
 *
 *  The cost model type for ranking linkages. Currently, there are two models: VDAL (:vdal)
 *  and CORPUS (:corpus). The VDAL model ranks parses from lowest to highest cost in and-cost,
 *  disjunct-cost, unused-word-cost and structure-violations-cost. The CORPUS model ranks
 *  parses according to the frequency of use of disjuncts, based on a statistical analysis
 *  of a collection of texts. If you haven't compiled the link-grammar library with support
 *  for the CORPUS cost model, attempting to set it to this will raise an exception.
 *
 */
static VALUE
rlink_parseopts_set_cost_model_type( VALUE self, VALUE model_name )
{
	Parse_Options opts = get_parseopts( self );
	Cost_Model_type model;

	if ( model_name == vdal_sym ) {
		rlink_log_obj( self, "debug", "Selected the 'VDAL' cost model" );
		model = VDAL;
	}
#ifdef CORPUS
	else if ( model_name == corpus_sym ) {
		rlink_log_obj( self, "debug", "Selected the 'CORPUS' cost model" );
		model = CORPUS;
	}
	else {
		rb_raise( rb_eArgError, "Unknown cost model %s (expected either :vdal or :corpus).",
			 RSTRING_PTR(rb_inspect( model_name )) );
	}
#else
	else {
		rb_raise( rb_eArgError, "Unknown cost model %s (this system supports only :vdal).",
			 RSTRING_PTR(rb_inspect( model_name )) );
	}
#endif // CORPUS

	rlink_log_obj( self, "info", "Setting the cost model to %s", model == VDAL ? "VDAL" : "CORPUS" );
	parse_options_reset_resources( opts );
	parse_options_set_cost_model_type( opts, model );

	if ( parse_options_get_cost_model_type(opts) != model ) {
		rb_raise( rb_eArgError,
			"Couldn't set the cost model: is link-grammar possibly compiled without it?" );
	}

	return model_name;
}


/*
 *  call-seq:
 *     opts.cost_model_type   -> Symbol
 *
 *  Get the cost model type for ranking linkages.
 */
static VALUE
rlink_parseopts_get_cost_model_type( VALUE self )
{
	Parse_Options opts = get_parseopts( self );
	Cost_Model_type model = parse_options_get_cost_model_type( opts );
	VALUE model_name = Qnil;

	switch( model ) {
	case VDAL:
		model_name = vdal_sym;
		break;
#ifdef CORPUS
	case CORPUS:
		model_name = corpus_sym;
		break;
#endif // CORPUS
	default:
		rb_bug( "Unhandled cost model type %d", model );
	}

	return model_name;
}


/*
 *  call-seq:
 *     opts.spell_guessing_enabled= boolean
 *
 *  Enable/disable spell-guessing if it's supported.
 */
static VALUE
rlink_parseopts_set_spell_guess( VALUE self, VALUE val )
{
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
rlink_parseopts_get_spell_guess_p( VALUE self )
{
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
rlink_parseopts_timer_expired_p( VALUE self )
{
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
rlink_parseopts_memory_exhausted_p( VALUE self )
{
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
rlink_parseopts_resources_exhausted_p( VALUE self )
{
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
rlink_parseopts_reset_resources( VALUE self )
{
	Parse_Options opts = get_parseopts( self );

	parse_options_reset_resources( opts );
	return Qnil;
}



void
rlink_init_parseoptions()
{
	rlink_cParseOptions = rb_define_class_under( rlink_mLinkParser,
		"ParseOptions", rb_cObject );

	vdal_sym = ID2SYM( rb_intern("vdal") );
	corpus_sym = ID2SYM( rb_intern("corpus") );

	rb_define_alloc_func( rlink_cParseOptions, rlink_parseopts_s_alloc );
	rb_define_method( rlink_cParseOptions, "initialize", rlink_parseopts_init, -1 );
	rb_define_method( rlink_cParseOptions, "initialize_copy", rlink_parseopts_init_copy, 1 );
/*
	rb_define_method( rlink_cParseOptions, "merge", rlink_parseopts_merge, 1 );
	rb_define_method( rlink_cParseOptions, "merge!", rlink_parseopts_merge_bang, 1 );
*/
	rb_define_method( rlink_cParseOptions, "verbosity=", rlink_parseopts_set_verbosity, 1 );
	rb_define_method( rlink_cParseOptions, "verbosity", rlink_parseopts_get_verbosity, 0 );
	rb_define_method( rlink_cParseOptions, "linkage_limit=", rlink_parseopts_set_linkage_limit, 1 );
	rb_define_method( rlink_cParseOptions, "linkage_limit", rlink_parseopts_get_linkage_limit, 0 );
	rb_define_method( rlink_cParseOptions, "disjunct_cost=", rlink_parseopts_set_disjunct_cost, 1 );
	rb_define_method( rlink_cParseOptions, "disjunct_cost", rlink_parseopts_get_disjunct_cost, 0 );
	rb_define_method( rlink_cParseOptions, "min_null_count=", rlink_parseopts_set_min_null_count, 1 );
	rb_define_method( rlink_cParseOptions, "min_null_count", rlink_parseopts_get_min_null_count, 0 );
	rb_define_method( rlink_cParseOptions, "max_null_count=", rlink_parseopts_set_max_null_count, 1 );
	rb_define_method( rlink_cParseOptions, "max_null_count", rlink_parseopts_get_max_null_count, 0 );
	rb_define_method( rlink_cParseOptions, "islands_ok=", rlink_parseopts_set_islands_ok, 1 );
	rb_define_method( rlink_cParseOptions, "islands_ok?", rlink_parseopts_get_islands_ok_p, 0 );
	rb_define_method( rlink_cParseOptions, "short_length=", rlink_parseopts_set_short_length, 1 );
	rb_define_method( rlink_cParseOptions, "short_length", rlink_parseopts_get_short_length, 0 );
	rb_define_method( rlink_cParseOptions, "max_memory=", rlink_parseopts_set_max_memory, 1 );
	rb_define_method( rlink_cParseOptions, "max_memory", rlink_parseopts_get_max_memory, 0 );
	rb_define_method( rlink_cParseOptions, "max_parse_time=", rlink_parseopts_set_max_parse_time, 1 );
	rb_define_method( rlink_cParseOptions, "max_parse_time", rlink_parseopts_get_max_parse_time, 0 );
	rb_define_method( rlink_cParseOptions, "all_short_connectors=", rlink_parseopts_set_all_short_connectors, 1 );
	rb_define_method( rlink_cParseOptions, "all_short_connectors?", rlink_parseopts_get_all_short_connectors_p, 0 );
	rb_define_method( rlink_cParseOptions, "cost_model_type=", rlink_parseopts_set_cost_model_type, 1 );
	rb_define_method( rlink_cParseOptions, "cost_model_type", rlink_parseopts_get_cost_model_type, 0 );

	rb_define_method( rlink_cParseOptions, "spell_guessing_enabled=", rlink_parseopts_set_spell_guess, 1 );
	rb_define_method( rlink_cParseOptions, "spell_guessing_enabled?", rlink_parseopts_get_spell_guess_p, 0 );

	rb_define_method( rlink_cParseOptions, "timer_expired?", rlink_parseopts_timer_expired_p, 0 );
	rb_define_method( rlink_cParseOptions, "memory_exhausted?", rlink_parseopts_memory_exhausted_p, 0 );
	rb_define_method( rlink_cParseOptions, "resources_exhausted?", rlink_parseopts_resources_exhausted_p, 0 );
	rb_define_method( rlink_cParseOptions, "reset_resources", rlink_parseopts_reset_resources, 0 );

}

