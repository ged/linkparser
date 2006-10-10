#!/usr/bin/ruby
#
#	Module Install Script
#	$Id: install.rb 11 2005-08-07 03:30:22Z ged $
#
#	Thanks to Masatoshi SEKI for ideas found in his install.rb.
#
#	Copyright (c) 2001-2005 The FaerieMUD Consortium.
#
#	This is free software. You may use, modify, and/or redistribute this
#	software under the terms of the Perl Artistic License. (See
#	http://language.perl.com/misc/Artistic.html)
#

require './utils.rb'
include UtilityFunctions

require 'rbconfig'
include Config

require 'pathname'
require 'find'
require 'ftools'
require 'optparse'

$version	= %q$Revision: 11 $
$rcsId		= %q$Id: install.rb 11 2005-08-07 03:30:22Z ged $

# Define required libraries
RequiredLibraries = []

class Installer

	@@PrunePatterns = [
		/CVS/,
		/~$/,
		%r:(^|/)\.:,
		/\.tpl$/,
		/\.[cho]$/,
	]

	def initialize( testing=false )
		@ftools = (testing) ? self : File
	end

	### Make the specified dirs (which can be a String or an Array of Strings)
	### with the specified mode.
	def makedirs( dirs, mode=0755, verbose=false )
		dirs = [ dirs ] unless dirs.is_a? Array

		oldumask = File::umask
		File::umask( 0777 - mode )

		for dir in dirs
			if @ftools == File
				File::mkpath( dir, $verbose )
			else
				$stderr.puts "Make path %s with mode %o" % [ dir, mode ]
			end
		end

		File::umask( oldumask )
	end

	def install( srcfile, dstfile, mode=nil, verbose=false )
		dstfile = File.catname(srcfile, dstfile)
		unless FileTest.exist? dstfile and File.cmp srcfile, dstfile
			$stderr.puts "   install #{srcfile} -> #{dstfile}"
		else
			$stderr.puts "   skipping #{dstfile}: unchanged"
		end
	end

	public

	def installFiles( src, dstDir, mode=0444, verbose=false )
		directories = []
		files = []
		
		if File.directory?( src )
			Find.find( src ) {|f|
				Find.prune if @@PrunePatterns.find {|pat| f =~ pat}
				next if f == src

				if FileTest.directory?( f )
					directories << f.gsub( /^#{src}#{File::Separator}/, '' )
					next 

				elsif FileTest.file?( f )
					files << f.gsub( /^#{src}#{File::Separator}/, '' )

				else
					Find.prune
				end
			}
		else
			files << File.basename( src )
			src = File.dirname( src )
		end
		
		dirs = [ dstDir ]
		dirs |= directories.collect {|d| File.join(dstDir,d)}
		makedirs( dirs, 0755, verbose )
		files.each {|f|
			srcfile = File.join(src,f)
			dstfile = File.dirname(File.join( dstDir,f ))

			if verbose
				if mode
					$stderr.puts "Install #{srcfile} -> #{dstfile} (mode %o)" % mode
				else
					$stderr.puts "Install #{srcfile} -> #{dstfile}"
				end
			end

			@ftools.install( srcfile, dstfile, mode, verbose )
		}
	end

end


if $0 == __FILE__
	dryrun = false
	linkgrammar_dir = '/usr/local'

	# Parse command-line switches
	ARGV.options {|oparser|
		oparser.banner = "Usage: #$0 [options]\n"

		oparser.on( "--with-linkgrammar-dir=DIRECTORY", '-d=DIRECTORY', String,
		 	"Specify the directory where linkgrammar is installed." ) do |dir|
			linkgrammar_dir = dir
		end

		oparser.on( "--verbose", "-v", TrueClass, "Make progress verbose" ) {
			$VERBOSE = true
			debugMsg "Turned verbose on."
		}

		oparser.on( "--dry-run", "-n", TrueClass, "Don't really install anything" ) {
			debugMsg "Turned dry-run on."
			dryrun = true
		}

		# Handle the 'help' option
		oparser.on( "--help", "-h", "Display this text." ) {
			$stderr.puts oparser
			exit!(0)
		}

		oparser.parse!
	}

	debugMsg "Sitelibdir = '#{CONFIG['sitelibdir']}'"
	sitelibdir = CONFIG['sitelibdir']
	debugMsg "Sitearchdir = '#{CONFIG['sitearchdir']}'"
	sitearchdir = CONFIG['sitearchdir']
	dlext = CONFIG['DLEXT']

	extpath = Pathname.new( "ext/linkparser_ext.#{dlext}" )
	unless extpath.exist?
		message "Building #{extpath}...\n"
		Kernel.load( "extconf.rb", "--with-linkgrammar-dir=#{linkgrammar_dir}" )
		
		system( 'make' )
		status = $?
		
		if not status.success?
			puts "Build failed.\n\n"
			exit status.exitstatus
		end
	end
	
	testflag = Pathname.new( ".tests.passed" )
	unless testflag.exist?
		message "Testing...\n"
		require "./test.rb"
		result = runtests()
		
		if not result.passed?
			response = promptWithDefault "Some tests failed. Install anyway?", 'n'
			exit unless response =~ /^y/i
		else
			testflag.open( "w" ) do |f|
				f.puts( result.to_s )
			end
		end
	end

	message "Installing...\n"
	i = Installer.new( dryrun )
	i.installFiles( extpath, sitearchdir, 0444, $VERBOSE )
	i.installFiles( "lib", sitelibdir, 0444, $VERBOSE )

	message "done.\n"
end
	



