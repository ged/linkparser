#!/usr/bin/env ruby

require 'mkmf'

ADDITIONAL_LIBRARY_DIRS = %w[
	/usr/local/lib
	/opt/lib
	/opt/local/lib
]
ADDITIONAL_INCLUDE_DIRS = %w[
	/usr/local/include
	/opt/include
	/opt/local/include
]

$CFLAGS << ' -Wall' << ' -DDEBUG'

def fail( *messages )
	$stderr.puts( *messages )
	exit 1
end


dir_config( 'link-grammar' )


find_library( "link-grammar", "dictionary_create", *ADDITIONAL_LIBRARY_DIRS ) or
	fail( "Could not find Link-Grammar library", 
	      "(http://www.abisource.com/projects/link-grammar/#download)." )
find_header( "link-grammar/link-includes.h", *ADDITIONAL_INCLUDE_DIRS ) or
	fail( "Could not find link-includes.h" )

unless have_func( "linkage_free_diagram", 'link-grammar/link-includes.h' )
	message "Your link-grammar library is too old for this binding.\n",
	        "Please upgrade to the latest version posted here:\n",
	        "  http://www.abisource.com/projects/link-grammar/#download\n",
	        "and try again.\n"
	fail "No linkage_free_diagram() in the installed link-grammar."
end

unless have_func( "linkage_get_current_sublinkage" )
	message "Link grammar library is unpatched. Building anyways.\n"
	message "  LinkParser::Linkage#current_sublinkage will not work,\n"
	message "  but everything else will work okay.\n"
end

unless have_func( "linkgrammar_get_version" )
	message "You are building with an older, unsupported version of the link-grammar\n"
	message " library. If you encounter problems, please update it to 4.4.0 or later.\n"
	message "Trying to build anyway.\n"
end

create_makefile( 'linkparser_ext' )
