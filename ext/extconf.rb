#!/usr/bin/env ruby

require 'mkmf'

if lgdir = with_config( 'link-grammar' )
	ENV['PATH'] = "#{lgdir}/bin" + File::PATH_SEPARATOR + ENV['PATH']
end

### Read the output of a command using the fork+pipe syntax so execution errors 
### propagate to Ruby.
def read_cmd_output( *cmd )
	output = IO.read( '|-' ) or exec( *cmd )
	return output.chomp
end

pkgconfig = with_config( 'pkg-config' ) || 'pkg-config'
pkgconfig = find_executable( pkgconfig ) or
	fail "Couldn't find your pkg-config binary"

$LDFLAGS << read_cmd_output( pkgconfig, '--libs-only-L', 'link-grammar' )
$CFLAGS << read_cmd_output( pkgconfig, '--cflags', 'link-grammar' )

# Sort out the universal vs. single-archicture build problems on MacOS X
if RUBY_PLATFORM.include?( 'darwin' )
	puts "MacOS X build: fixing architecture flags:"

	# Only keep the '-arch <a>' flags present in both the cflags reported
	# by pkg-config and those that Ruby specifies.
	commonflags = nil
	if ENV['ARCHFLAGS']
		puts "  using the value in ARCHFLAGS environment variable (%p)." % [ ENV['ARCHFLAGS'] ]
		commonflags = ENV['ARCHFLAGS']
	elsif pkgconfig
		puts "  finding flags common to both Ruby and link-grammar..."
		archflags = []
		pkgcflags = read_cmd_output( pkgconfig, '--cflags', 'link-grammar' )
		pkgcflags.scan( /-arch\s+(\S+)/ ).each do |arch|
			puts "  testing for architecture: %p" % [ arch ]
			archflags << "-arch #{arch}" if Config::CONFIG['CFLAGS'].index("-arch #{arch}")
		end

		commonflags = archflags.join(' ')
		puts "  common arch flags: %s" % [ commonflags ]
	else
		$stderr.puts %{
		===========   WARNING   ===========
		
		You are building this extension on OS X without setting the 
		ARCHFLAGS environment variable, and pkg-config wasn't found in 
		your PATH. If you are seeing this message, that means that the 
		build will probably fail.

		If it does, you can correct this by either including the path 
		to 'pkg-config' in your PATH or setting the environment variable 
		ARCHFLAGS to '-arch <arch>' before building.

		For example:
		(in bash) $ export PATH=/opt/local/bin:$PATH                  
		          $ export ARCHFLAGS='-arch x86_64'
		(in tcsh) % set path = ( /opt/local/bin $PATH )
		          % setenv ARCHFLAGS '-arch x86_64'

		Then try building again.

		===================================
		}.gsub( /^\t+/, '  ' )
	end

	if commonflags
		$CFLAGS.gsub!( /-arch\s+\S+ /, '' )
		$LDFLAGS.gsub!( /-arch\s+\S+ /, '' )
		CONFIG['LDSHARED'].gsub!( /-arch\s+\S+ /, '' )

		$CFLAGS << ' ' << commonflags
		$LDFLAGS << ' ' << commonflags
		CONFIG['LDSHARED'] << ' ' << commonflags
	end
end


dir_config( 'link-grammar' )


find_library( "link-grammar", "dictionary_create" ) or
	fail( "Could not find Link-Grammar library", 
	      "(http://www.abisource.com/projects/link-grammar/#download)." )
find_header( "link-grammar/link-includes.h" ) or
	fail( "Could not find link-includes.h" )

unless have_func( "linkage_is_fat", 'link-grammar/link-includes.h' )
	message "Your link-grammar library is too old for this binding.\n",
	        "Please upgrade to the latest version posted here:\n",
	        "  http://www.abisource.com/projects/link-grammar/#download\n",
	        "and try again.\n"
	fail "No linkage_is_fat() in the installed link-grammar."
end

have_func( 'parse_options_get_spell_guess' )
have_func( 'linkage_get_disjunct_str' )

create_makefile( 'linkparser_ext' )

