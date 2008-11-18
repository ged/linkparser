#!/usr/bin/env ruby

require 'mkmf'

def fail( *messages )
	$stderr.puts( *messages )
	exit 1
end


dir_config( 'link-grammar' )

$CFLAGS << ' -Wall'
$CFLAGS << ' -DDEBUG'

have_header( "link-grammar/link-includes.h" ) or
	fail( "Could not find link-includes.h" )

message "Checking for dictionary_create()"
have_library( "link-grammar", "dictionary_create", 'link-grammar/link-includes.h' ) or
	fail( "Could not find link-grammar library." )

message "Checking for a recent-enough version of link-grammar\n"
unless have_func( "linkage_free_diagram", 'link-grammar/link-includes.h' )
	log "Your link-grammar library is too old for this binding.\n",
	    "Please upgrade to the latest version posted here:\n",
	    "  http://www.abisource.com/projects/link-grammar/#download\n",
	    "and try again.\n"
	fail "No linkage_free_diagram() in the installed link-grammar."
end

message "Checking for patched link-grammar library via linkage_get_current_sublinkage()\n"
if have_func( "linkage_get_current_sublinkage" )
	message "Link grammar library has linkage_get_current_sublinkage(). Okay, I'll use it.\n"
else
	message "Link grammar library is unpatched. Building without it.\n"
end

create_makefile( 'linkparser_ext' )
