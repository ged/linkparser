#!/usr/bin/ruby
# 
# A test script to check if dictionaries are freed before the program terminates
# 

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent.expand_path
	extdir = basedir + "ext"
	
	$LOAD_PATH.unshift( extdir ) unless $LOAD_PATH.include?( extdir )
	
	require basedir + "utils.rb"
	include UtilityFunctions
}


$DEBUG = 1
require 'linkparser'

$deferr.sync = true

1000.times do
	$deferr.print '.'
	
	dict = LinkParser::Dictionary.new
	GC.start
end

