#!/usr/bin/ruby
#
# $Id$
# Time-stamp: <28-Nov-2004 00:30:32 ged>
#
# Authors:
#   # Michael Granger <ged@FaerieMUD.org>
#
# Copyright (c) 2006 The FaerieMUD Consortium.
#
# This work is licensed under the Creative Commons Attribution License. To
# view a copy of this license, visit
# http://creativecommons.org/licenses/by/1.0 or send a letter to Creative
# Commons, 559 Nathan Abbott Way, Stanford, California 94305, USA.
#

require 'mkmf'

def abort( msg )
	$stderr.puts( msg )
	exit 1
end


$CFLAGS << ' -Wall'
$CFLAGS << ' -DDEBUG'

dir_config( "linkgrammar" )
have_library( "link-grammar", "dictionary_create" ) or
	abort( "Could not find link-grammar library." )
have_header( "link-grammar/link-includes.h" )
have_header( "link-grammar/utilities.h" )
have_func( "linkage_get_current_sublinkage", "link-grammar/link-includes.h" ) or
	warn "Link grammar library is unpatched."

create_makefile( "linkparser_ext" )


