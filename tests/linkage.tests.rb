#!/usr/bin/ruby -w
#
# Unit tests for the LinkParser::Linkage class
# $Id$
#
# Copyright (c) 2006 The FaerieMUD Consortium
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

class LinkParser::Linkage::TestCase < Test::Unit::TestCase

	def setup
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
		@simple_sentence = @dict.parse( "The flag was wet." )
		@conjunct_sentence = 
			@dict.parse( "The ball rolled down the hill and bumped the curb." )
	end


	def test_diagram_should_return_a_diagram_string
		rval = nil
		
		assert_nothing_raised do
			rval = @simple_sentence.linkages.first.diagram
		end
		
		assert_match( /LEFT-WALL/, rval )
		assert_match( /the/, rval )
		assert_match( /flag\.n/, rval )
		assert_match( /was\.v/, rval )
		assert_match( /wet\.a/, rval )

		assert_match( /-Xp-/, rval )
		assert_match( /-Wd-/, rval )
		assert_match( /-Ds-/, rval )
		assert_match( /-Ss-/, rval )
		assert_match( /-Pa-/, rval )
		
	end
	
	
	def test_links_and_domains_should_return_string_of_links_and_domains
		rval = nil
		
		assert_nothing_raised do
			rval = @simple_sentence.linkages.first.links_and_domains
		end
		
		assert_match( /LEFT-WALL/, rval )
		assert_match( /the/, rval )
		assert_match( /flag\.n/, rval )
		assert_match( /was\.v/, rval )
		assert_match( /wet\.a/, rval )

		assert_match( /-Xp-/, rval )
		assert_match( /-Wd-/, rval )
		assert_match( /-Ds-/, rval )
		assert_match( /-Ss-/, rval )
		assert_match( /-Pa-/, rval )
	end
	
	def test_num_sublinkages_should_return_conjunction_count
		rval = nil
		
		assert_nothing_raised do
			rval = @conjunct_sentence.linkages.first.num_sublinkages
		end
		
		assert_equal 2, rval, "2 conjunctions"
	end
	
	
end

