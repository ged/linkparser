#!/usr/bin/ruby -w
#
# Unit tests for the #{vars[:class_under_test]} class
# $Id$
#
# Copyright (c) #{date.year} #{tm[:organization_name]}
# 
# This work is licensed under the Creative Commons Attribution-ShareAlike
# License. To view a copy of this license, visit
# http://creativecommons.org/licenses/by-sa/1.0/ or send a letter to Creative
# Commons, 559 Nathan Abbott Way, Stanford, California 94305, USA.
#
# 

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent
	
	$LOAD_PATH.unshift( basedir + 'ext' ) unless 
		$LOAD_PATH.include?( basedir + 'ext' )
}

require 'test/unit'
require 'linkparser'

class #{vars[:class_under_test]}::TestCase < Test::Unit::TestCase

	def test_something
		
	end
	
end

