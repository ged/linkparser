/*
 *  linkage.c - Ruby LinkParser Linkage class
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
static rlink_LINKAGE *
rlink_linkage_alloc()
{
	rlink_LINKAGE *ptr = ALLOC( rlink_LINKAGE );
	
	ptr->linkage	= NULL;
	ptr->sentence	= Qnil;
	
	debugMsg(( "Initialized an rlink_LINKAGE <%p>", ptr ));
	return ptr;
}


/*
 * GC Mark function
 */
static void
rlink_linkage_gc_mark( ptr )
	rlink_LINKAGE *ptr;
{
	debugMsg(( "Marking LinkParser::Linkage %p", ptr ));
	
	if ( ptr ) {
		rb_gc_mark( ptr->sentence );
	}
	
	else {
		debugMsg(( "Not marking uninitialized rlink_LINKAGE" ));
	}
}


/*
 * GC Free function
 */
static void
rlink_linkage_gc_free( ptr )
	rlink_LINKAGE *ptr;
{
	if ( ptr ) {
		linkage_delete( (Linkage)ptr->linkage );
		ptr->linkage = NULL;
		ptr->sentence = Qnil;
	}
	
	else {
		debugMsg(( "Not freeing an uninitialized rlink_LINKAGE" ));
	}
}


/*
 * Object validity checker. Returns the data pointer.
 */
static rlink_LINKAGE *
check_linkage( self )
	 VALUE	self;
{
	debugMsg(( "Checking a LinkParser::Linkage object (%d).", self ));
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
static rlink_LINKAGE *
get_linkage( self )
	 VALUE self;
{
	rlink_LINKAGE *ptr = check_linkage( self );

	debugMsg(( "Fetching a Linkage (%p).", ptr ));
	if ( !ptr )
		rb_raise( rb_eRuntimeError, "uninitialized Linkage" );

	return ptr;
}


/*
 * Publicly-usable linkage-fetcher
 */
/*rlink_LINKAGE *
rlink_get_linkage( self )
{
	return get_linkage( self );
}
*/


/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 * allocate()
 * --
 * Allocate a new LinkParser::Linkage object.
 */
static VALUE
rlink_linkage_s_alloc( klass )
	 VALUE klass;
{
	debugMsg(( "Wrapping an uninitialized Linkage pointer." ));
	return Data_Wrap_Struct( klass, rlink_linkage_gc_mark, rlink_linkage_gc_free, 0 );
}


/* --------------------
 * Instance methods
 * -------------------- */


/*
 * initialize( index, sentence, options={} )
 * --
 * Create a new LinkParser::Linkage object out of the linkage indicated by
 * +index+ (a positive Integer) from the specified sentence (a 
 * LinkParser::Sentence). The optional options hash can be used to override
 * the parse options of the Sentence for the new linkage.
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
		rlink_SENTENCE *sent_ptr;
		Linkage linkage;
		Parse_Options opts;
		rlink_LINKAGE *ptr;
		
		i = rb_scan_args( argc, argv, "21", &index, &sentence, &options );

		defopts = rb_hash_new(); /*rb_funcall( sentence, rb_intern("options"), 0 );*/
		options = rlink_make_parse_options( defopts, options );
		opts = rlink_get_parseopts( options );

		sent_ptr = (rlink_SENTENCE *)rlink_get_sentence( sentence );

		link_index = NUM2INT(index);
		max_index = sentence_num_linkages_found((Sentence)sent_ptr->sentence) - 1;
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



/* 
 * diagram
 * --
 * Return a String containing a diagram of the linkage.
 */
static VALUE
rlink_linkage_diagram( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char *diagram_cstr;
	VALUE diagram;
	
	diagram_cstr = linkage_print_diagram( (Linkage)ptr->linkage );
	diagram = rb_str_new2( diagram_cstr );
	string_delete( diagram_cstr );
	
	return diagram;
}


/* 
 * postscript_diagram( full_doc=false )
 * --
 * Returns the macros needed to print out the linkage in a postscript file. 
 * By default, the output is just the set of postscript macros that describe 
 * the diagram. With full_doc=true a complete encapsulated postscript document 
 * is returned.
 */
static VALUE
rlink_linkage_print_postscript( self, full_doc )
	VALUE self, full_doc;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char *diagram_cstr;
	VALUE diagram;
	
	diagram_cstr = linkage_print_postscript( (Linkage)ptr->linkage,
		RTEST(full_doc) ? 1 : 0 );
	diagram = rb_str_new2( diagram_cstr );
	string_delete( diagram_cstr );
	
	return diagram;
}


/* 
 * links_and_domains
 * --
 * Return a String containing a lists all of the links and domain names for 
 * the current sublinkage. For example, for the sentence "I eat, therefore I 
 * think":
 * 
 *             /////          RW      <---RW---->  RW        /////
 *   (m)       /////          Wd      <---Wd---->  Wd        I.p
 *   (m)       I.p            CC      <---CC---->  CC        therefore
 *   (m)       I.p            Sp*i    <---Sp*i-->  Sp        eat
 *   (m)       ,              Xd      <---Xd---->  Xd        therefore
 *   (m) (m)   therefore      Wd      <---Wd---->  Wd        I.p
 *   (m) (m)   I.p            Sp*i    <---Sp*i-->  Sp        think.v
 * 
 * 
 * 
 */
static VALUE
rlink_linkage_links_and_domains( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char *diagram_cstr;
	VALUE diagram;
	
	diagram_cstr = linkage_print_links_and_domains( (Linkage)ptr->linkage );
	diagram = rb_str_new2( diagram_cstr );
	string_delete( diagram_cstr );
	
	return diagram;
}



/* 
 * num_sublinkages
 * --
 * Return the number of sublinkages for a linkage with conjunctions, 1 
 * otherwise.
 */
static VALUE
rlink_linkage_num_sublinkages( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return INT2FIX( linkage_get_num_sublinkages((Linkage)ptr->linkage) );
}


/*
 * num_words
 * --
 * The number of words in the sentence for which this is a linkage. Note that 
 * this function does not return the number of words used in the current 
 * sublinkage.
 */
static VALUE
rlink_linkage_get_num_words( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return INT2FIX( linkage_get_num_words((Linkage)ptr->linkage) );
}


/*
 * num_links
 * --
 * The number of links used in the current sublinkage.
 */
static VALUE
rlink_linkage_get_num_links( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return INT2FIX( linkage_get_num_links((Linkage)ptr->linkage) );
}


/*
 * link_lword( index )
 * --
 * The number of the word on the left end of the index-th link of the 
 * current sublinkage.
 */
static VALUE
rlink_linkage_get_link_lword( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	
	return INT2FIX( linkage_get_link_lword((Linkage)ptr->linkage, i) );
}


/*
 * link_rword( index )
 * --
 * The number of the word on the right end of the index-th link of the 
 * current sublinkage.
 */
static VALUE
rlink_linkage_get_link_rword( self, arg )
	VALUE self, arg;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	
	return INT2FIX( linkage_get_link_rword((Linkage)ptr->linkage, i) );
}


/*
 * link_length( index )
 * --
 * The number of words spanned by the index-th link of the current sublinkage.
 */
static VALUE
rlink_linkage_get_link_length( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	
	return INT2FIX( linkage_get_link_length((Linkage)ptr->linkage, i) );
}


/*
 * link_label( index ) => str
 * --
 * The "intersection" of the left and right connectors that comprise the link.
 */
static VALUE
rlink_linkage_get_link_label( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	char *label;
	
	label = linkage_get_link_label( (Linkage)ptr->linkage, i );
	return rb_str_new2( label );
}


/*
 * link_llabel => str
 * --
 * The label on the left word of the index-th link of the current sublinkage.
 */
static VALUE
rlink_linkage_get_link_llabel( self, arg )
	VALUE self, arg;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	char *label;
	
	label = linkage_get_link_llabel( (Linkage)ptr->linkage, i );
	return rb_str_new2( label );
}

/*
 * link_rlabel => str
 * --
 * The label on the right word of the index-th link of the current sublinkage.
 */
static VALUE
rlink_linkage_get_link_rlabel( self, arg )
	VALUE self, arg;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	char *label;
	
	label = linkage_get_link_rlabel( (Linkage)ptr->linkage, i );
	return rb_str_new2( label );
}


/*
 * link_num_domains( index ) => fixnum
 * --
 * The number of domains in the index-th link.
 */
static VALUE
rlink_linkage_get_link_num_domains( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = FIX2INT( index );
	int count = 0;
	
	count = linkage_get_link_num_domains( (Linkage)ptr->linkage, i );
	return INT2FIX( count );
}


/*
 * link_domain_names => array
 * --
 * The names of the domains the index-th link belongs to.
 */
static VALUE
rlink_linkage_get_link_domain_names( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char **names;
	int i = FIX2INT( index );
	int count;
	VALUE names_ary;
	
	names = linkage_get_link_domain_names( (Linkage)ptr->linkage, i );
	count = linkage_get_link_num_domains( (Linkage)ptr->linkage, i );
	names_ary = rb_ary_new2( count );
	
	for ( i = 0; i < count; i++ ) {
		rb_ary_store( names_ary, i, rb_str_new2(names[i]) );
	}
	
	return names_ary;
}


/*
 * words => array
 * --
 * Return the Array of word spellings or individual word spelling for the 
 * current sublinkage. These are the "inflected" spellings, such as "dog.n". 
 * The original spellings can be obtained by calls to Sentence#words.
 */
static VALUE
rlink_linkage_get_words( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char **words;
	int count, i;
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
 * compute_union
 * --
 * If the linkage has a conjunction, combine all of the links occurring in all
 * sublinkages together -- in effect creating a "master" linkage (which may
 * have crossing links). The union is created as another sublinkage, thus
 * increasing the number of sublinkages by one, and is returned by this method.
 * If the linkage has no conjunctions, computing its union has no effect, and
 * nil is returned.
 */
static VALUE
rlink_linkage_compute_union( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval;
	
	rval = linkage_compute_union( (Linkage)ptr->linkage );
	
	return INT2FIX( rval );
}

/*
 * unused_word_cost
 * --
 * 
 */
static VALUE
rlink_linkage_unused_word_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}

/*
 * disjunct_cost
 * --
 * 
 */
static VALUE
rlink_linkage_disjunct_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}

/*
 * and_cost
 * --
 * 
 */
static VALUE
rlink_linkage_and_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}

/*
 * link_cost
 * --
 * 
 */
static VALUE
rlink_linkage_link_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}

/*
 * canonical?
 * --
 * 
 */
static VALUE
rlink_linkage_canonical_p( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}


/*
 * improper?
 * --
 * 
 */
static VALUE
rlink_linkage_improper_p( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}


/*
 * has_inconsistent_domains?
 * --
 * 
 */
static VALUE
rlink_linkage_has_inconsistent_domains_p( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}


/*
 * violation_name
 * --
 * 
 */
static VALUE
rlink_linkage_get_violation_name( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return Qnil;
}



/* Class initializer */
void
rlink_init_linkage(void)
{
	rlink_cLinkage = rb_define_class_under( rlink_mLinkParser, "Linkage", rb_cObject );
	
	rb_define_alloc_func( rlink_cLinkage, rlink_linkage_s_alloc );
	
	rb_define_method( rlink_cLinkage, "initialize", rlink_linkage_init, -1 );
	rb_define_method( rlink_cLinkage, "diagram", rlink_linkage_diagram, 0 );
	rb_define_method( rlink_cLinkage, "postscript_diagram",
	 	rlink_linkage_print_postscript, 1 );
	rb_define_method( rlink_cLinkage, "links_and_domains", 
		rlink_linkage_links_and_domains, 0 );

	rb_define_method( rlink_cLinkage, "num_sublinkages", 
		rlink_linkage_num_sublinkages, 0 );
	
	rb_define_method( rlink_cLinkage, "num_words",
	 	rlink_linkage_get_num_words, 0 );
	rb_define_method( rlink_cLinkage, "num_links",
	 	rlink_linkage_get_num_links, 0 );
	
	rb_define_method( rlink_cLinkage, "link_lword",
	 	rlink_linkage_get_link_lword, 1 );
	rb_define_method( rlink_cLinkage, "link_rword",
	 	rlink_linkage_get_link_rword, 1 );
	rb_define_method( rlink_cLinkage, "link_length",
	 	rlink_linkage_get_link_length, 1 );
	rb_define_method( rlink_cLinkage, "link_label",
	 	rlink_linkage_get_link_label, 1 );
	rb_define_method( rlink_cLinkage, "link_llabel",
	 	rlink_linkage_get_link_llabel, 1 );
	rb_define_method( rlink_cLinkage, "link_rlabel",
	 	rlink_linkage_get_link_rlabel, 1 );

	rb_define_method( rlink_cLinkage, "link_num_domains",
	 	rlink_linkage_get_link_num_domains, 1 );
	rb_define_method( rlink_cLinkage, "link_domain_names",
	 	rlink_linkage_get_link_domain_names, 1 );
	
	rb_define_method( rlink_cLinkage, "words",
	 	rlink_linkage_get_words, 0 );

	rb_define_method( rlink_cLinkage, "compute_union",
	 	rlink_linkage_compute_union, 0 );
	rb_define_method( rlink_cLinkage, "unused_word_cost",
	 	rlink_linkage_unused_word_cost, 0 );
	rb_define_method( rlink_cLinkage, "disjunct_cost",
	 	rlink_linkage_disjunct_cost, 0 );
	rb_define_method( rlink_cLinkage, "and_cost",
	 	rlink_linkage_and_cost, 0 );
	rb_define_method( rlink_cLinkage, "link_cost",
	 	rlink_linkage_link_cost, 0 );
	rb_define_method( rlink_cLinkage, "canonical?",
	 	rlink_linkage_canonical_p, 0 );
	rb_define_method( rlink_cLinkage, "improper?",
	 	rlink_linkage_improper_p, 0 );
	rb_define_method( rlink_cLinkage, "has_inconsistent_domains?",
	 	rlink_linkage_has_inconsistent_domains_p, 0 );
	rb_define_method( rlink_cLinkage, "violation_name",
	 	rlink_linkage_get_violation_name, 0 );
	

/*	rb_define_method( rlink_cLinkage, "print_constituent_tree",
	 	rlink_linkage_print_constituent_tree, 1 );
*/

}

