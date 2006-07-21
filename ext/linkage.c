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



/* Class initializer */
void
rlink_init_linkage(void)
{
	rlink_cLinkage = rb_define_class_under( rlink_mLinkParser, "Linkage", rb_cObject );
	
	rb_define_alloc_func( rlink_cLinkage, rlink_linkage_s_alloc );
	
	rb_define_method( rlink_cLinkage, "initialize", rlink_linkage_init, -1 );
	rb_define_method( rlink_cLinkage, "diagram", rlink_linkage_diagram, 0 );
	rb_define_method( rlink_cLinkage, "diagram", rlink_linkage_diagram, 0 );
	rb_define_method( rlink_cLinkage, "links_and_domains", 
		rlink_linkage_links_and_domains, 0 );
}

