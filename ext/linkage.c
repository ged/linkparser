/*
 *  linkage.c - Ruby LinkParser Linkage class
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
 * Macros and constants
 * -------------------------------------------------- */

VALUE display_walls_sym;
VALUE display_header_sym;
VALUE max_width_sym;


/* --------------------------------------------------
 *	Memory-management functions
 * -------------------------------------------------- */
/*
 * Allocation function
 */
static struct rlink_linkage *
rlink_linkage_alloc()
{
	struct rlink_linkage *ptr = ALLOC( struct rlink_linkage );

	ptr->linkage	= NULL;
	ptr->sentence	= Qnil;

	rlink_log( "debug", "Initialized an rlink_LINKAGE <%p>", ptr );
	return ptr;
}


/*
 * GC Mark function
 */
static void
rlink_linkage_gc_mark( struct rlink_linkage *ptr )
{
	if ( ptr ) {
		rb_gc_mark( ptr->sentence );
	}
}


/*
 * GC Free function
 */
static void
rlink_linkage_gc_free( struct rlink_linkage *ptr )
{
	if ( ptr ) {
		linkage_delete( (Linkage)ptr->linkage );
		ptr->linkage = NULL;
		ptr->sentence = Qnil;

		xfree( ptr );
		ptr = NULL;
	}
}


/*
 * Object validity checker. Returns the data pointer.
 */
static struct rlink_linkage *
check_linkage( VALUE self )
{
	Check_Type( self, T_DATA );

    if ( !IsLinkage(self) ) {
		rb_raise( rb_eTypeError, "wrong argument type %s (expected LinkParser::Linkage)",
				  rb_class2name(CLASS_OF( self )) );
    }

	return DATA_PTR( self );
}


/*
 * Fetch the data pointer and check it for sanity.
 */
static struct rlink_linkage *
get_linkage(  VALUE self )
{
	struct rlink_linkage *ptr = check_linkage( self );

	if ( !ptr )
		rb_raise( rb_eRuntimeError, "uninitialized Linkage" );

	return ptr;
}


/*
 * Publicly-usable linkage-fetcher
 */
struct rlink_linkage *
rlink_get_linkage( VALUE self )
{
	return get_linkage( self );
}



/*
 *  call-seq:
 *     LinkParser::Linkage.allocate   -> LinkParser::Linkage
 *
 *  Allocate a new LinkParser::Linkage object.
 */
static VALUE
rlink_linkage_s_alloc(  VALUE klass )
{
	rlink_log( "debug", "Wrapping an uninitialized Linkage pointer." );
	return Data_Wrap_Struct( klass, rlink_linkage_gc_mark, rlink_linkage_gc_free, 0 );
}


/*
 *  call-seq:
 *     new( index, sentence, options={} )   -> LinkParser::Linkage
 *
 *  Create a new LinkParser::Linkage object out of the linkage indicated by
 *  +index+ (a positive Integer) from the specified sentence (a
 *  LinkParser::Sentence). The optional options hash can be used to override
 *  the parse options of the Sentence for the new linkage.
 */
static VALUE
rlink_linkage_init( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
	if ( !check_linkage(self) ) {
		int i, link_index, max_index;
		VALUE index, sentence, options, defopts;
		struct rlink_sentence *sent_ptr;
		Linkage linkage;
		Parse_Options opts;
		struct rlink_linkage *ptr;

		i = rb_scan_args( argc, argv, "21", &index, &sentence, &options );

		defopts = rb_hash_new(); /*rb_funcall( sentence, rb_intern("options"), 0 );*/
		options = rlink_make_parse_options( defopts, options );
		opts = rlink_get_parseopts( options );

		sent_ptr = (struct rlink_sentence *)rlink_get_sentence( sentence );

		link_index = NUM2INT(index);
		max_index = sentence_num_valid_linkages((Sentence)sent_ptr->sentence) - 1;
		if ( link_index > max_index )
			rb_raise( rlink_eLpError, "Invalid linkage %d (max is %d)",
				link_index, max_index );

		linkage = linkage_create( link_index, (Sentence)sent_ptr->sentence, opts );
		if ( !linkage ) rlink_raise_lp_error();

		DATA_PTR( self ) = ptr = rlink_linkage_alloc();

		ptr->linkage = linkage;
		ptr->sentence = sentence;
	}

	else {
		rb_raise( rb_eRuntimeError,
				  "Cannot re-initialize a linkage once it's been created." );
	}

	return Qnil;
}


// char * linkage_print_diagram(const Linkage linkage, bool display_walls, size_t screen_width);
// void   linkage_free_diagram(char * str);
//
// char * linkage_print_postscript(const Linkage linkage, bool display_walls, bool print_ps_header);
// void   linkage_free_postscript(char * str);
//
// char * linkage_print_links_and_domains(const Linkage linkage);
// void   linkage_free_links_and_domains(char *str);
//
// char * linkage_print_disjuncts(const Linkage linkage);
// void   linkage_free_disjuncts(char *str);


/*
 *  call-seq:
 *     diagram( display_walls: true, max_width: 80 )   -> str
 *
 *  Return a String containing a diagram of the linkage. If +display_walls+ is +true+
 *  the diagram will include the wall-words and connections to them. Strings longer
 *  than +max_width+ will be wrapped at that width.
 */
static VALUE
rlink_linkage_diagram( int argc, VALUE *argv, VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	char *diagram_cstr;
	bool display_walls = true;
	size_t screen_width = 80;
	VALUE opthash = Qnil,
	      diagram = Qnil;

	rb_scan_args( argc, argv, "0:", &opthash );
	if ( opthash != Qnil ) {
		rlink_log_obj( self, "debug", "Got an opthash: %s", RSTRING_PTR(rb_inspect(opthash)) );
		display_walls = RTEST( rb_hash_lookup2(opthash, display_walls_sym, Qtrue) );
		if ( rb_hash_lookup(opthash, max_width_sym) != Qnil ) {
			screen_width = NUM2UINT( rb_hash_lookup(opthash, max_width_sym) );
		}
	}

	rlink_log_obj( self, "debug", "Display walls: %d, screen_width: %d", display_walls, screen_width );

	diagram_cstr = linkage_print_diagram( (Linkage)ptr->linkage, display_walls, screen_width );
	diagram = rb_str_new2( diagram_cstr );
	linkage_free_diagram( diagram_cstr );

	return diagram;
}


/*
 *  call-seq:
 *     postscript_diagram( display_walls: true, display_header: false )   -> str
 *
 *  Returns the macros needed to print out the linkage in a postscript file.
 *  By default, the output is just the set of postscript macros that describe
 *  the diagram. With full_doc=true a complete encapsulated postscript document
 *  is returned.
 */
static VALUE
rlink_linkage_print_postscript( int argc, VALUE *argv, VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	char *diagram_cstr;
	bool display_walls = true,
	     display_header = false;
	VALUE opthash = Qnil,
	      diagram = Qnil;

	rb_scan_args( argc, argv, "0:", &opthash );
	if ( opthash != Qnil ) {
		rlink_log_obj( self, "debug", "Got an opthash: %s", RSTRING_PTR(rb_inspect(opthash)) );
		display_walls = RTEST( rb_hash_lookup2(opthash, display_walls_sym, Qtrue) );
		display_header = RTEST( rb_hash_lookup2(opthash, display_header_sym, Qfalse) );
	}

	rlink_log_obj( self, "debug", "Display walls: %d, display_header: %d", display_walls,
		display_header );

	diagram_cstr = linkage_print_postscript( (Linkage)ptr->linkage, display_walls, display_header );
	diagram = rb_str_new2( diagram_cstr );
	linkage_free_postscript( diagram_cstr );

	return diagram;
}


/*
 *  call-seq:
 *     links_and_domains   -> str
 *
 *  Return a String containing a lists all of the links and domain names for
 *  the current sublinkage.
 *
 *  Example:
 *    sent = dict.parse("I eat, therefore I think")
 *    puts sent.linkages.first.links_and_domains
 *
 *  prints:
 *              /////          RW      <---RW---->  RW        /////
 *    (m)       /////          Wd      <---Wd---->  Wd        I.p
 *    (m)       I.p            CC      <---CC---->  CC        therefore
 *    (m)       I.p            Sp*i    <---Sp*i-->  Sp        eat
 *    (m)       ,              Xd      <---Xd---->  Xd        therefore
 *    (m) (m)   therefore      Wd      <---Wd---->  Wd        I.p
 *    (m) (m)   I.p            Sp*i    <---Sp*i-->  Sp        think.v
 *
 */
static VALUE
rlink_linkage_links_and_domains( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	char *diagram_cstr;
	VALUE diagram;

	diagram_cstr = linkage_print_links_and_domains( (Linkage)ptr->linkage );
	diagram = rb_str_new2( diagram_cstr );
	linkage_free_links_and_domains( diagram_cstr );

	return diagram;
}


/*
 * num_words
 * --
 * The number of words in the sentence for which this is a linkage. Note that
 * this function does not return the number of words used in the current
 * sublinkage.
 */
static VALUE
rlink_linkage_get_num_words( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	return INT2FIX( linkage_get_num_words((Linkage)ptr->linkage) );
}


/*
 * num_links
 * --
 * The number of links used in the current sublinkage.
 */
static VALUE
rlink_linkage_get_num_links( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	return INT2FIX( linkage_get_num_links((Linkage)ptr->linkage) );
}


/*
 * link_lword( index )
 * --
 * The number of the word on the left end of the index-th link of the
 * current sublinkage.
 */
static VALUE
rlink_linkage_get_link_lword( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );

	return INT2FIX( linkage_get_link_lword((Linkage)ptr->linkage, i) );
}


/*
 * link_rword( index )
 * --
 * The number of the word on the right end of the index-th link of the
 * current sublinkage.
 */
static VALUE
rlink_linkage_get_link_rword( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );

	return INT2FIX( linkage_get_link_rword((Linkage)ptr->linkage, i) );
}


/*
 * link_length( index )
 * --
 * The number of words spanned by the index-th link of the current sublinkage.
 */
static VALUE
rlink_linkage_get_link_length( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );

	return INT2FIX( linkage_get_link_length((Linkage)ptr->linkage, i) );
}


/*
 * link_label( index ) -> str
 * --
 * The "intersection" of the left and right connectors that comprise the link.
 */
static VALUE
rlink_linkage_get_link_label( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );
	const char *label;

	label = linkage_get_link_label( (Linkage)ptr->linkage, i );
	if ( !label ) return Qnil;

	return rb_str_new2( label );
}


/*
 * link_llabel -> str
 * --
 * The label on the left word of the index-th link of the current sublinkage.
 */
static VALUE
rlink_linkage_get_link_llabel( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );
	const char *label = NULL;

	label = linkage_get_link_llabel( (Linkage)ptr->linkage, i );
	if ( !label ) return Qnil;

	return rb_str_new2( label );
}

/*
 * link_rlabel -> str
 * --
 * The label on the right word of the index-th link of the current sublinkage.
 */
static VALUE
rlink_linkage_get_link_rlabel( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );
	const char *label = NULL;

	label = linkage_get_link_rlabel( (Linkage)ptr->linkage, i );
	if ( !label ) return Qnil;

	return rb_str_new2( label );
}


/*
 *  disjunct_strings -> array
 *
 *  Return an Array of Strings showing the disjuncts that were actually used in association
 *  with each corresponding word in the current linkage. Each string shows the disjuncts
 *  in proper order; that is, left-to-right, in the order in which they link to other words.
 *  The returned strings can be thought of as a very precise part-of-speech-like label for
 *  each word, indicating how it was used in the given sentence; this can be useful
 *  for corpus statistics.
 *
 *  For a parsed version of the disjunct strings, call #disjuncts instead.
 *
 */
static VALUE
rlink_linkage_get_disjunct_strings( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	const char *disjunct;
	unsigned long i, count = 0l;
	VALUE disjuncts_ary;

	count = linkage_get_num_words( (Linkage)ptr->linkage );
	disjuncts_ary = rb_ary_new2( count );

	for ( i = 0; i < count; i++ ) {
#ifdef HAVE_LINKAGE_GET_DISJUNCT_STR
		disjunct = linkage_get_disjunct_str( (Linkage)ptr->linkage, i );
#else
		disjunct = linkage_get_disjunct( (Linkage)ptr->linkage, i );
#endif
		if ( disjunct ) {
			rb_ary_store( disjuncts_ary, i, rb_str_new2(disjunct) );

		} else {
			rb_ary_store( disjuncts_ary, i, Qnil );
		}
	}

	return disjuncts_ary;
}


/*
 *  call-seq:
 *     link_num_domains( index )   -> fixnum
 *
 *  Returns the number of domains in the index-th link.
 *
 */
static VALUE
rlink_linkage_get_link_num_domains( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int i = NUM2INT( index );
	int count = 0;

	count = linkage_get_link_num_domains( (Linkage)ptr->linkage, i );
	return INT2FIX( count );
}


/*
 *  call-seq:
 *     link_domain_names( index )   -> array
 *
 *  Returns the names of the domains the index-th link belongs to.
 */
static VALUE
rlink_linkage_get_link_domain_names( VALUE self, VALUE index )
{
	struct rlink_linkage *ptr = get_linkage( self );
	const char **names;
	int i = NUM2INT( index );
	int count;
	VALUE names_ary;

	names = linkage_get_link_domain_names( (Linkage)ptr->linkage, i );
	count = linkage_get_link_num_domains( (Linkage)ptr->linkage, i );
	if ( count < 0 ) return rb_ary_new();

	names_ary = rb_ary_new2( count );

	for ( i = 0; i < count; i++ ) {
		rb_ary_store( names_ary, i, rb_str_new2(names[i]) );
	}

	return names_ary;
}


/*
 *  call-seq:
 *     words   -> array
 *
 *  Return the Array of word spellings or individual word spelling for the
 *  current sublinkage. These are the "inflected" spellings, such as "dog.n".
 *  The original spellings can be obtained by calls to Sentence#words.
 */
static VALUE
rlink_linkage_get_words( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	const char **words;
	unsigned long count, i;
	VALUE words_ary;

	count = linkage_get_num_words( (Linkage)ptr->linkage );
	words = linkage_get_words( (Linkage)ptr->linkage );
	words_ary = rb_ary_new2( count );

	for ( i = 0; i < count; i++ ) {
		rb_ary_store( words_ary, i, rb_str_new2(words[i]) );
	}

	return words_ary;
}


/*
 *  call-seq:
 *     linkage.unused_word_cost   -> fixnum
 *
 *  Returns the unused word cost of the linkage, which corresponds to the number
 *  of null links that were required to parse it.
 *
 */
static VALUE
rlink_linkage_unused_word_cost( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int rval;

	rval = linkage_unused_word_cost( (Linkage)ptr->linkage );

	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     linkage.disjunct_cost   -> fixnum
 *
 *  Returns the connector or disjunct cost of the linkage.
 *
 */
static VALUE
rlink_linkage_disjunct_cost( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int rval;

	rval = linkage_disjunct_cost( (Linkage)ptr->linkage );

	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     linkage.link_cost   -> fixnum
 *
 *  Returns the total (LEN) cost of the linkage, which is the total length of
 *  all links in the sentence minus the number of words -- since the total link
 *  length is never less than the number of words.
 *
 */
static VALUE
rlink_linkage_link_cost( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	int rval;

	rval = linkage_link_cost( (Linkage)ptr->linkage );

	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     linkage.violation_name   -> str
 *
 *  If the linkage violated any post-processing rules, this method returns the
 *  name of the violated rule in the post-process knowledge file.
 */
static VALUE
rlink_linkage_get_violation_name( VALUE self )
{
	struct rlink_linkage *ptr = get_linkage( self );
	const char *violation_name = NULL;

	violation_name = linkage_get_violation_name( (Linkage)ptr->linkage );

	if ( violation_name ) {
		return rb_str_new2( violation_name );
	} else {
		return Qnil;
	}
}


void
rlink_init_linkage()
{
	rlink_cLinkage = rb_define_class_under( rlink_mLinkParser, "Linkage", rb_cObject );

	display_walls_sym  = ID2SYM( rb_intern("display_walls") );
	display_header_sym = ID2SYM( rb_intern("display_header") );
	max_width_sym      = ID2SYM( rb_intern("max_width") );

	rb_define_alloc_func( rlink_cLinkage, rlink_linkage_s_alloc );

	rb_define_method( rlink_cLinkage, "initialize", rlink_linkage_init, -1 );
	rb_define_method( rlink_cLinkage, "diagram", rlink_linkage_diagram, -1 );
	rb_define_method( rlink_cLinkage, "postscript_diagram", rlink_linkage_print_postscript, -1 );
	rb_define_method( rlink_cLinkage, "links_and_domains", rlink_linkage_links_and_domains, 0 );

	rb_define_method( rlink_cLinkage, "num_words", rlink_linkage_get_num_words, 0 );
	rb_define_alias ( rlink_cLinkage, "word_count", "num_words" );
	rb_define_method( rlink_cLinkage, "num_links", rlink_linkage_get_num_links, 0 );
	rb_define_alias ( rlink_cLinkage, "link_count", "num_links" );

	rb_define_method( rlink_cLinkage, "link_lword", rlink_linkage_get_link_lword, 1 );
	rb_define_method( rlink_cLinkage, "link_rword", rlink_linkage_get_link_rword, 1 );
	rb_define_method( rlink_cLinkage, "link_length", rlink_linkage_get_link_length, 1 );
	rb_define_method( rlink_cLinkage, "link_label", rlink_linkage_get_link_label, 1 );
	rb_define_method( rlink_cLinkage, "link_llabel", rlink_linkage_get_link_llabel, 1 );
	rb_define_method( rlink_cLinkage, "link_rlabel", rlink_linkage_get_link_rlabel, 1 );

	rb_define_method( rlink_cLinkage, "link_num_domains", rlink_linkage_get_link_num_domains, 1 );
	rb_define_method( rlink_cLinkage, "link_domain_names", rlink_linkage_get_link_domain_names, 1 );

	rb_define_method( rlink_cLinkage, "words", rlink_linkage_get_words, 0 );
	rb_define_method( rlink_cLinkage, "disjunct_strings", rlink_linkage_get_disjunct_strings, 0 );

	rb_define_method( rlink_cLinkage, "unused_word_cost", rlink_linkage_unused_word_cost, 0 );
	rb_define_method( rlink_cLinkage, "disjunct_cost", rlink_linkage_disjunct_cost, 0 );
	rb_define_method( rlink_cLinkage, "link_cost", rlink_linkage_link_cost, 0 );
	rb_define_method( rlink_cLinkage, "violation_name", rlink_linkage_get_violation_name, 0 );
}

