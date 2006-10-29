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
	require basedir + 'linkparser-path.rb'
}

require 'test/unit'
require 'linkparser'

class LinkParser::Linkage::TestCase < Test::Unit::TestCase

	@dict = LinkParser::Dictionary.new( :verbosity => 0 )
	class << self; attr_accessor :dict; end

	def setup
		@simple_sentence = self.class.dict.parse( "The flag was wet." )
		@ss_linkage = @simple_sentence.linkages.first
		@conjunct_sentence = 
			self.class.dict.parse( "The ball rolled down the hill and bumped the curb." )
	end

        def test_linkage_current_sublinkage_should_return
          rval = nil
          assert_nothing_raised do
            rval = @ss_linkage.current_sublinkage
          end
          assert_kind_of(Fixnum, rval)
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

    #1         LEFT-WALL      Xp      <---Xp---->  Xp        .
    #2   (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
    #3+4 (m)   the            D       <---Ds---->  Ds        flag.n
    #5   (m)   flag.n         Ss      <---Ss---->  Ss        was.v
    #6   (m)   was.v          Pa      <---Pa---->  Pa        wet.a
	def test_linkage_links_should_contain_link_structs_describing_the_linkage
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.links
		end
		
		assert_instance_of Array, rval
		assert_kind_of Struct, rval.first
		assert_equal 'LEFT-WALL', rval.first.lword, "left word of first link"
		assert_equal 'Xp', rval.first.label, "label of first link"
		assert_equal 'RIGHT-WALL', rval.last.rword, "right word of last link"
		assert_equal 'RW', rval.last.label, "label of last link"
		assert_equal 'flag.n', rval[3].lword, "left word of the fourth link"
		assert_equal 'was.v', rval[3].rword, "right word of the fourth link"
		assert_equal 'Ss', rval[3].label, "label of the fourth link"
	end


	def test_linkage_verb_should_return_sentence_verb
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.verb
		end
		
		assert_equal "was", rval
	end


	# The ball rolled down the hill and bumped the curb.
	def test_linkage_verb_should_return_vword_of_current_sublinkage_of_conjunction
		rval = nil
		linkage = @conjunct_sentence.linkages.first
		
		assert_nothing_raised do
			rval = linkage.verb
		end
		
		assert_equal 'rolled', rval
		
		assert_nothing_raised do
			linkage.current_sublinkage = 1
			rval = linkage.verb
		end
		
		assert_equal 'bumped', rval
	end

	
	
	Mode1CTreeString = "(S (NP The flag)\n   (VP was\n       (ADJP wet))\n   .)\n"
	Mode2CTreeString = "[S [NP The flag NP] [VP was [ADJP wet ADJP] VP] . S] \n"
	Mode3CTreeString = "(S (NP The flag) (VP was (ADJP wet)) .)\n"
	
	def test_constituent_tree_string_without_mode_should_return_mode1_string
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.constituent_tree_string
		end
		
		assert_equal Mode1CTreeString, rval
	end


	def test_constituent_tree_string_with_mode1_should_return_mode1_string
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.constituent_tree_string( 1 )
		end
		
		assert_equal Mode1CTreeString, rval
	end

	def test_constituent_tree_string_with_mode2_should_return_mode2_string
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.constituent_tree_string( 2 )
		end
		
		assert_equal Mode2CTreeString, rval
	end

	def test_constituent_tree_string_with_mode3_should_return_mode3_string
		rval = nil
		
		assert_nothing_raised do
			rval = @ss_linkage.constituent_tree_string( 3 )
		end
		
		assert_equal Mode3CTreeString, rval
	end

	def test_constituent_tree_string_with_mode4_should_raise_exception
		assert_raises( ArgumentError ) do
			@ss_linkage.constituent_tree_string( 4 )
		end
	end

	def test_constituent_tree_string_with_nonnumeric_should_raise_exception
		assert_raises( TypeError ) do
			@ss_linkage.constituent_tree_string( "Glah" )
		end
	end


	def test_constituent_tree_should_return_array_of_CTree_structs
		rval = nil
		
		assert_nothing_raised do 
			rval = @ss_linkage.constituent_tree
		end
		
		assert_kind_of Array, rval
		assert_kind_of Struct, rval.first
		assert_equal "S", rval.first.label
		assert_equal "NP", rval.first.children.first.label
		assert_equal "The", rval.first.children.first.children.first.label
	end
	
end

