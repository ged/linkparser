/*
 *  linkage.c - Ruby LinkParser Linkage class
 *  $Id$
 *  
 *  Authors:
 *    * Michael Granger <ged@FaerieMUD.org>
 *  
 *  Copyright (c) 2007 The FaerieMUD Consortium
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *  
 */

#include "linkparser.h"


/* --------------------------------------------------
 *  Forward declarations
 * -------------------------------------------------- */

static VALUE rlink_linkage_make_cnode_array( CNode * );


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
rlink_LINKAGE *
rlink_get_linkage( self )
{
	return get_linkage( self );
}



/* --------------------------------------------------
 * Class Methods
 * -------------------------------------------------- */

/*
 *  call-seq:
 *     LinkParser::Linkage.allocate   -> LinkParser::Linkage
 *
 *  Allocate a new LinkParser::Linkage object.
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



/*
 *  call-seq:
 *     diagram   -> str
 *
 *  Return a String containing a diagram of the linkage.
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
 *  call-seq:
 *     postscript_diagram( full_doc=false )   -> str
 *
 *  Returns the macros needed to print out the linkage in a postscript file. 
 *  By default, the output is just the set of postscript macros that describe 
 *  the diagram. With full_doc=true a complete encapsulated postscript document 
 *  is returned.
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
 *  call-seq:
 *     num_sublinkages   -> fixnum
 *
 *  Return the number of sublinkages for a linkage with conjunctions, 1 
 *  otherwise.
 */
static VALUE
rlink_linkage_num_sublinkages( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	return INT2FIX( linkage_get_num_sublinkages((Linkage)ptr->linkage) );
}


/*
 *  call-seq:
 *     current_sublinkage = index   -> true or false
 *
 *  After this call, all operations on the linkage will refer to the index-th 
 *  sublinkage. In the case of a linkage without conjunctions, this has no 
 *  effect.
 */
static VALUE
rlink_linkage_current_sublinkage_eq( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval = 0;
	
	rval = linkage_set_current_sublinkage( (Linkage)ptr->linkage, NUM2INT(index) );
	
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     current_sublinkage   -> fixnum
 *
 *  Get the index of the current sublinkage.
 */
static VALUE
rlink_linkage_current_sublinkage( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval = 0;

	rval = linkage_get_current_sublinkage( (Linkage)ptr->linkage );
	
	return INT2FIX( rval );
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
rlink_linkage_get_link_rword( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = NUM2INT( index );
	
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
	int i = NUM2INT( index );
	
	return INT2FIX( linkage_get_link_length((Linkage)ptr->linkage, i) );
}


/*
 * link_label( index ) -> str
 * --
 * The "intersection" of the left and right connectors that comprise the link.
 */
static VALUE
rlink_linkage_get_link_label( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = NUM2INT( index );
	char *label;
	
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
rlink_linkage_get_link_llabel( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = NUM2INT( index );
	char *label = NULL;
	
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
rlink_linkage_get_link_rlabel( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int i = NUM2INT( index );
	char *label = NULL;
	
	label = linkage_get_link_rlabel( (Linkage)ptr->linkage, i );
	if ( !label ) return Qnil;
	
	return rb_str_new2( label );
}


/*
 *  call-seq:
 *     link_num_domains( index )   -> fixnum
 *
 *  Returns the number of domains in the index-th link.
 *
 */
static VALUE
rlink_linkage_get_link_num_domains( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
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
rlink_linkage_get_link_domain_names( self, index )
	VALUE self, index;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char **names;
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
 *  call-seq:
 *     compute_union   -> true or false
 *
 *  If the linkage has a conjunction, combine all of the links occurring in all
 *  sublinkages together -- in effect creating a "master" linkage (which may
 *  have crossing links). The union is created as another sublinkage, thus
 *  increasing the number of sublinkages by one, and is returned by this method.
 *  If the linkage has no conjunctions, computing its union has no effect. This 
 *  method returns true if computing its union caused another sublinkage to be
 *  created.
 */
static VALUE
rlink_linkage_compute_union( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int before, after;
	
	before = linkage_get_num_sublinkages( (Linkage)ptr->linkage );
	linkage_compute_union( (Linkage)ptr->linkage );
	after = linkage_get_num_sublinkages( (Linkage)ptr->linkage );
	
	return (after > before) ? Qtrue : Qfalse;
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
rlink_linkage_unused_word_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
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
rlink_linkage_disjunct_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval;
	
	rval = linkage_disjunct_cost( (Linkage)ptr->linkage );
	
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     linkage.and_cost   -> fixnum
 *
 *  Returns the AND cost of the linkage, which is the difference in length 
 *  between and-list elements.
 *
 */
static VALUE
rlink_linkage_and_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval;
	
	rval = linkage_and_cost( (Linkage)ptr->linkage );
	
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
rlink_linkage_link_cost( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval;
	
	rval = linkage_link_cost( (Linkage)ptr->linkage );
	
	return INT2FIX( rval );
}


/*
 *  call-seq:
 *     linkage.canonical?   -> true or false
 *
 *  Returns +true+ if the linkage is canonical. The canonical linkage is the 
 *  one in which the minimal disjunct that ever occurrs in a position is used 
 *  in that position.
 */
static VALUE
rlink_linkage_canonical_p( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval = 0;
	
	rval = linkage_is_canonical( (Linkage)ptr->linkage );
	
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     linkage.improper?   -> true or false
 *
 *  Returns +true+ if the linkage is "improper". 
 *  --
 *  :FIXME: Find out what an "improper fat linkage" is.
 *
 */
static VALUE
rlink_linkage_improper_p( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval = 0;
	
	rval = linkage_is_improper( (Linkage)ptr->linkage );
	
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     linkage.has_inconsistent_domains?   -> true or false
 *
 *  Returns +true+ if the linkage has inconsistent domains. 
 *  --
 *  :FIXME: Find out what it means that a linkage has inconsistent domains.
 *
 */
static VALUE
rlink_linkage_has_inconsistent_domains_p( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	int rval = 0;
	
	rval = linkage_has_inconsistent_domains( (Linkage)ptr->linkage );
	
	return rval ? Qtrue : Qfalse;
}


/*
 *  call-seq:
 *     linkage.violation_name   -> str
 *
 *  If the linkage violated any post-processing rules, this method returns the 
 *  name of the violated rule in the post-process knowledge file. 
 */
static VALUE
rlink_linkage_get_violation_name( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char *violation_name = NULL;
	
	violation_name = linkage_get_violation_name( (Linkage)ptr->linkage );
	
	if ( violation_name ) {
		return rb_str_new2( violation_name );
	} else {
		return Qnil;
	}
}


/*
 *  call-seq:
 *     linkage.constituent_tree   -> hash
 *
 *  Return the Linkage's constituent tree as a hash of hashes.
 *
 *     sent = dict.parse( "He is a big dog." )
 *     link = sent.linkages.first
 *     ctree = link.constituent_tree
 *     #=> {}
 *     
 */
static VALUE
rlink_linkage_constituent_tree( self )
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	CNode *ctree = NULL;
	VALUE rval = Qnil;
	
	ctree = linkage_constituent_tree( (Linkage)ptr->linkage );
	rval = rlink_linkage_make_cnode_array( ctree );
	
	linkage_free_constituent_tree( ctree );
	return rval;
}

static VALUE
rlink_linkage_make_cnode_array( ctree )
	CNode *ctree;
{
	VALUE nodes = rb_ary_new();
	VALUE rnode;
	CNode *cnode = ctree;
	
	/*	
		struct CNode_s {
		  char  * label;
		  CNode * child;
		  CNode * next;
		  int   start, end;
		};
	*/
	while ( cnode ) {
		rnode = rb_struct_new( rlink_sLinkageCTree,
			rb_str_new2( linkage_constituent_node_get_label(cnode) ),
			Qnil,
			INT2FIX( linkage_constituent_node_get_start(cnode) ),
			INT2FIX( linkage_constituent_node_get_end(cnode) )			/* end */
		  );

		/* Make a node array for any children */
		rb_struct_aset( rnode, INT2FIX(1), 
			rlink_linkage_make_cnode_array(linkage_constituent_node_get_child(cnode)) );

		rb_ary_push( nodes, rnode );
		cnode = linkage_constituent_node_get_next( cnode );
	}
	
	return nodes;
}


/*
 *  call-seq:
 *     linkage.constituent_tree_string( mode=1 )   -> str
 *
 *  Return the constituent tree as a printable string.
 *
 *  Example:
 *     sent = dict.parse( "He is a big dog." )
 *     link = sent.linkages.first
 *     link.constituent_tree_string
 #  
 #  # ==> "(S (NP He)\n   (VP is\n       (NP a big dog))\n   .)\n"
 */
static VALUE
rlink_linkage_constituent_tree_string( argc, argv, self )
	int argc;
	VALUE *argv;
	VALUE self;
{
	rlink_LINKAGE *ptr = get_linkage( self );
	char *ctree_string = NULL;
	VALUE rval = Qnil, modenum = Qnil;
	int mode;
	
	if ( rb_scan_args(argc, argv, "01", &modenum) == 1 ) {
		mode = NUM2INT( modenum );
	} else {
		mode = 1;
	}

	if ( mode < 1 || mode > 3 )
		rb_raise( rb_eArgError, "Illegal mode %d specified.", mode );

	ctree_string = linkage_print_constituent_tree( (Linkage)ptr->linkage, mode );

	if ( ctree_string ) {
		rval = rb_str_new2( ctree_string );
		string_delete( ctree_string );
	} else {
		rval = Qnil;
	}
	
	return rval;
}



/* 
 * This is the API's representation of a parse. A LinkParser::Sentence may have one or more
 * of LinkParser::Linkages, each of which represents one possible structure of the sentence.
 * It can be thought of as a Sentence together with a collection of links. If the parse 
 * has a conjunction, then the Linkage is made up of at least two "sublinkages". A 
 * Linkage can be pretty printed in either ASCII or Postscript format, and individual 
 * links can be extracted.
 *
 */
void
rlink_init_linkage(void)
{
#ifdef FOR_RDOC
	rlink_mLinkParser = rb_define_module( "LinkParser" );
	rlink_eLpError = rb_define_class_under( rlink_mLinkParser, "Error", rb_eRuntimeError );
#endif

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
	rb_define_method( rlink_cLinkage, "current_sublinkage=",
		rlink_linkage_current_sublinkage_eq, 1 );
	rb_define_method( rlink_cLinkage, "current_sublinkage",
		rlink_linkage_current_sublinkage, 0 );
	
	rb_define_method( rlink_cLinkage, "num_words",
	 	rlink_linkage_get_num_words, 0 );
	rb_define_alias ( rlink_cLinkage, "word_count", "num_words" );
	rb_define_method( rlink_cLinkage, "num_links",
	 	rlink_linkage_get_num_links, 0 );
	rb_define_alias ( rlink_cLinkage, "link_count", "num_links" );
	
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

	/* Struct that contains links of a constituent tree */
	rb_define_const( rlink_cLinkage, "CTree", rlink_sLinkageCTree );

	rlink_sLinkageCTree = rb_struct_define( "LinkParserLinkageCTree", 
		"label", "children", "start", "end", NULL );
	rb_define_method( rlink_cLinkage, "constituent_tree",
		rlink_linkage_constituent_tree, 0 );
	rb_define_method( rlink_cLinkage, "constituent_tree_string",
	 	rlink_linkage_constituent_tree_string, -1 );
}

