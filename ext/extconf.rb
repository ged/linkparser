#!/usr/bin/env ruby

require 'mkmf'

if lgdir = with_config( 'link-grammar' )
	ENV['PATH'] = "#{lgdir}/bin" + File::PATH_SEPARATOR + ENV['PATH']
end

pkgconfig = with_config( 'pkg-config' ) || 'pkg-config'
pkgconfig = find_executable( pkgconfig ) or
	abort "Couldn't find your pkg-config binary"

$LDFLAGS << ' ' + `#{pkgconfig} --libs-only-L link-grammar`.chomp
$CFLAGS <<  ' ' + `#{pkgconfig} --cflags link-grammar`.chomp


dir_config( 'link-grammar' )

find_library( "link-grammar", "parse_options_create" ) or
	abort "Could not find Link-Grammar library" +
	      "(http://www.abisource.com/projects/link-grammar/#download)."

find_header( "link-grammar/link-includes.h" ) or
	abort "Could not find link-includes.h"

if have_func( "parse_options_get_screen_width", 'link-grammar/link-includes.h' )
	$stderr.puts "Your link-grammar library is too old for this binding.",
	        "Please upgrade to the latest version posted here:",
	        "  http://www.abisource.com/projects/link-grammar/#download",
	        "and try again."
	abort "The parse_options_get_screen_width() still defined by the installed link-grammar."
end

have_func( 'dictionary_create' )
have_func( 'dictionary_create_lang' )
have_func( 'parse_options_get_spell_guess' )
have_func( 'linkage_get_disjunct_str' )

create_header()
create_makefile( 'linkparser_ext' )

