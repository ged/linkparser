#!/usr/bin/ruby

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname

	extdir = basedir + 'ext'
	libdir = basedir + 'lib'

	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
}

