#!/usr/bin/ruby

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname
	$LOAD_PATH.unshift( basedir + 'ext' ) unless 
		$LOAD_PATH.include?( basedir + 'ext' )
	$LOAD_PATH.unshift( basedir + 'lib' ) unless 
		$LOAD_PATH.include?( basedir + 'lib' )
}