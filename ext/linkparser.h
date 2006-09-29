/*
 *		linkparser.h - Ruby-LinkParser Header
 *		$Id$
 *
 *		Authors:
 *		  * Michael Granger <ged@FaerieMUD.org>
 *
 *		Copyright (c) 2006 The FaerieMUD Consortium.
 *
 *		This work is licensed under the Creative Commons Attribution License. To
 *		view a copy of this license, visit
 *		http://creativecommons.org/licenses/by/1.0 or send a letter to Creative
 *		Commons, 559 Nathan Abbott Way, Stanford, California 94305, USA.
 *
 */


#ifndef _R_LINKPARSER_H
#define _R_LINKPARSER_H

#include <stdlib.h>
#include <stdio.h>

#include <ruby.h>
#include <intern.h>				/* For rb_dbl2big() */

#include <link-grammar/link-includes.h>



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
typedef struct {
	Sentence 	sentence;
	VALUE	 	dictionary;
	VALUE		parsed_p;
	VALUE		options;
} rlink_SENTENCE;

typedef struct {
	Linkage		linkage;
	VALUE		sentence;
} rlink_LINKAGE;



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
extern void rlink_init_dict						_(( void ));
extern void rlink_init_sentence					_(( void ));
extern void rlink_init_linkage					_(( void ));
extern void rlink_init_parseoptions				_(( void ));

/* Fetchers */
extern Dictionary rlink_get_dict				_(( VALUE ));
extern rlink_SENTENCE *rlink_get_sentence		_(( VALUE ));
extern Parse_Options rlink_get_parseopts		_(( VALUE ));

#endif /* _R_LINKPARSER_H */

