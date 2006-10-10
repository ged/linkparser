#!/usr/bin/ruby
#
#	Test suite for LinkParser
#
#

BEGIN {
	require 'pathname'
	$basedir = Pathname.new( __FILE__ ).dirname
	require $basedir + "linkparser-path.rb"

	unless defined? UtilityFunctions
		require $basedir + "utils.rb"
	end
	include UtilityFunctions
}

require 'optparse'

# Turn off output buffering
$stderr.sync = $stdout.sync = true

# Initialize variables
safelevel = 0
patterns = []
requires = []
$DebugPattern = nil

# Parse command-line switches
ARGV.options {|oparser|
	oparser.banner = "Usage: #$0 [options] [TARGETS]\n"

	oparser.on( "--debug", "-d", FalseClass, "Turn debugging on" ) {
		$DEBUG = true
	}

	oparser.on( "--verbose", "-v", TrueClass, "Make progress verbose" ) {
		$VERBOSE = true
		debugMsg "Turned verbose on."
	}

	# Handle the 'help' option
	oparser.on( "--help", "-h", "Display this text." ) {
		$stderr.puts oparser
		exit!(0)
	}

	oparser.parse!
}

verboseOff {
	require 'find'
	require 'test/unit'
	require 'test/unit/testsuite'
	require 'test/unit/ui/console/testrunner'
}

# Parse test patterns
ARGV.each {|pat| patterns << Regexp.new( pat, Regexp::IGNORECASE )}
$stderr.puts "#{patterns.length} patterns given on the command line:",
    patterns.collect {|pat| "  " + pat.to_s }.join( "\n" )

### Load all the tests from the tests dir
Find.find( $basedir + "tests" ) {|file|
	Find.prune if /\/\./ =~ file or /~$/ =~ file
	Find.prune if /TEMPLATE/ =~ file
	next if File.stat( file ).directory?

 	unless patterns.empty?
 		Find.prune unless patterns.find {|pat| pat =~ file}
 	end

	debugMsg "Considering '%s': " % file
	next unless file =~ /\.tests.rb$/
	debugMsg "Requiring '%s'..." % file
	require "#{file}"
	requires << file
}

$stderr.puts "Required #{requires.length} files."
unless patterns.empty?
	$stderr.puts "[" + requires.sort.join( ", " ) + "]"
end

class LinkParserTests
	class << self
		def suite
			suite = Test::Unit::TestSuite.new( "Ruby-LinkParser" )

			if suite.respond_to?( :add )
				ObjectSpace.each_object( Class ) {|klass|
					suite.add( klass.suite ) if klass < Test::Unit::TestCase
				}
			else
				ObjectSpace.each_object( Class ) {|klass|
					suite << klass.suite if klass < Test::Unit::TestCase
				}			
			end

			return suite
		end
	end
end

def runtests
	Test::Unit::UI::Console::TestRunner.new( LinkParserTests ).start
end	

# Run tests
if $0 == __FILE__
	$SAFE = safelevel
	Dir.chdir( $basedir ) do
		runtests
	end
end


	

