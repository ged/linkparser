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

	@@dict = LinkParser::Dictionary.new( :verbosity => 0 )

	def setup
		@simple_sentence = @@dict.parse( "The flag was wet." )
		@ss_linkage = @simple_sentence.linkages.first
		@conjunct_sentence = 
			@@dict.parse( "The ball rolled down the hill and bumped the curb." )
	end


	def test_diagram_should_return_a_diagram_string
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.diagram
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
			rval = @ss_linkage.links_and_domains
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
		
		assert_equal 2, rval, "2 sublinkages"
	end
	
	def test_linkage_api_should_respond_to_num_words
		assert_respond_to @ss_linkage, :num_words
	end


	def test_simple_sentence_linkage_should_have_seven_words
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.num_words
		end
		
		assert_equal 7, rval, "words: %p" % 
			[ @ss_linkage.words ]
	end

	def test_linkage_api_should_respond_to_num_links
		assert_respond_to @ss_linkage, :num_links
	end

	def test_simple_sentence_linkage_should_have_six_links
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.num_links
		end
		
         #1         LEFT-WALL      Xp      <---Xp---->  Xp        .
         #2   (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
         #3+4 (m)   the            D       <---Ds---->  Ds        flag.n
         #5   (m)   flag.n         Ss      <---Ss---->  Ss        was.v
         #6   (m)   was.v          Pa      <---Pa---->  Pa        wet.a
		assert_equal 6, rval
	end
	
	def test_linkage_api_should_respond_to_link_lword
		assert_respond_to @ss_linkage, :link_lword
	end


	def test_linkage_api_should_respond_to_link_rword
		assert_respond_to @ss_linkage, :link_rword
	end


	def test_linkage_api_should_respond_to_link_length
		assert_respond_to @ss_linkage, :link_length
	end


	def test_linkage_api_should_respond_to_link_label
		assert_respond_to @ss_linkage, :link_label
	end


	def test_linkage_api_should_respond_to_link_llabel
		assert_respond_to @ss_linkage, :link_llabel
	end


	def test_linkage_api_should_respond_to_link_rlabel
		assert_respond_to @ss_linkage, :link_rlabel
	end


	def test_linkage_api_should_respond_to_link_num_domains
		assert_respond_to @ss_linkage, :link_num_domains
	end

	def test_linkage_api_should_respond_to_link_domain_names
		assert_respond_to @ss_linkage, :link_domain_names
	end
	
	def test_linkage_api_should_respond_to_words
		assert_respond_to @ss_linkage, :words
	end


	def test_linkage_api_should_respond_to_compute_union
		assert_respond_to @ss_linkage, :compute_union
	end


	def test_linkage_api_should_respond_to_unused_word_cost
		assert_respond_to @ss_linkage, :unused_word_cost
	end


	def test_linkage_api_should_respond_to_disjunct_cost
		assert_respond_to @ss_linkage, :disjunct_cost
	end


	def test_linkage_api_should_respond_to_and_cost
		assert_respond_to @ss_linkage, :and_cost
	end


	def test_linkage_api_should_respond_to_link_cost
		assert_respond_to @ss_linkage, :link_cost
	end


	def test_linkage_api_should_respond_to_canonical?
		assert_respond_to @ss_linkage, :canonical?
	end


	def test_linkage_api_should_respond_to_improper?
		assert_respond_to @ss_linkage, :improper?
	end


	def test_linkage_api_should_respond_to_has_inconsistent_domains?
		assert_respond_to @ss_linkage, :has_inconsistent_domains?
	end


	def test_linkage_api_should_respond_to_violation_name
		assert_respond_to @ss_linkage, :violation_name
	end

end

