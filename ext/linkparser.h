/*
 *		linkparser.h - Ruby-LinkParser Header
 *		$Id$
 *
 *		Authors:
 *		  * Michael Granger <ged@FaerieMUD.org>
 *
 *  Please see the LICENSE file at the top of the distribution for licensing 
 *  information.
 */


#ifndef _R_LINKPARSER_H
#define _R_LINKPARSER_H

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ruby.h"
#include "ruby/intern.h"

#include "link-grammar/link-includes.h"


/* Debugging functions/macros */
#ifdef HAVE_STDARG_PROTOTYPES
#include <stdarg.h>
#define va_init_list(a,b) va_start(a,b)
extern void rlink_debug(const char *fmt, ...);
#else
#include <varargs.h>
#define va_init_list(a,b) va_start(a)
extern void rlink_debug(fmt, va_alist);
#endif

extern void rlink_raise_lp_error _(( void ));
extern VALUE rlink_make_parse_options _(( VALUE, VALUE ));


/* -------------------------------------------------------
 * Globals
 * ------------------------------------------------------- */

/*
 * Modules
 */
extern VALUE rlink_mLinkParser;

extern VALUE rlink_cDictionary;
extern VALUE rlink_cSentence;
extern VALUE rlink_cLinkage;
extern VALUE rlink_cParseOptions;
extern VALUE rlink_cPostProcessor;
extern VALUE rlink_cConstituentTree;

extern VALUE rlink_sLinkageCTree;

extern VALUE rlink_eLpError;


/* 
 * Structures
 */
struct rlink_dictionary {
	Dictionary dict;
};

struct rlink_sentence {
	Sentence 	sentence;
	VALUE	 	dictionary;
	VALUE		parsed_p;
	VALUE		options;
};

struct rlink_linkage {
	Linkage		linkage;
	VALUE		sentence;
};



/*
 * Macros
 */

/* Debugging macro */
#if DEBUG
#  define debugMsg(f)	rlink_debug f
#else /* ! DEBUG */
#  define debugMsg(f) 
#endif /* DEBUG */


#define IsDictionary( obj ) rb_obj_is_kind_of( (obj), rlink_cDictionary )
#define IsSentence( obj ) rb_obj_is_kind_of( (obj), rlink_cSentence )
#define IsLinkage( obj ) rb_obj_is_kind_of( (obj), rlink_cLinkage )
#define IsParseOptions( obj ) rb_obj_is_kind_of( (obj), rlink_cParseOptions )


/* -------------------------------------------------------
 * Initializer functions
 * ------------------------------------------------------- */
extern void rlink_init_dict							_(( void ));
extern void rlink_init_sentence						_(( void ));
extern void rlink_init_linkage						_(( void ));
extern void rlink_init_parseoptions					_(( void ));

/* Fetchers */
extern struct rlink_dictionary * rlink_get_dict		_(( VALUE ));
extern struct rlink_sentence *rlink_get_sentence	_(( VALUE ));
extern Parse_Options rlink_get_parseopts			_(( VALUE ));

#endif /* _R_LINKPARSER_H */

