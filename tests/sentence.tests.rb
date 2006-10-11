#!/usr/bin/ruby -w
#
# Unit tests for the LinkParser::Sentence class
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
	require basedir + "linkparser-path.rb"
}


require 'test/unit'
require 'linkparser'

class LinkParser::Sentence::TestCase < Test::Unit::TestCase

	@dict = LinkParser::Dictionary.new( :verbosity => 0 )
	class << self; attr_accessor :dict; end


	def setup
		@dict = self.class.dict
		@sentence = LinkParser::Sentence.new( "The cat runs.", @dict )
	end
	

	def test_sentence_parse_returns_linkage_count
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.parse
		end
		
		assert_equal rval, 1, "'The cat runs.' should only have one linkage"
	end


	def test_sentence_parse_should_accept_parse_options
		rval = nil
		
		assert_nothing_raised do
			@sentence.parse( :verbosity => 0 )
		end
	end
	

	def test_sentence_length_should_return_word_count
		rval = nil
		assert_nothing_raised do
			rval = @sentence.length
		end
		assert_equal 6, rval, "'The cat runs.' should have a length of 6"
	end


	def test_sentence_delegates_linkage_methods_to_first_linkage
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.num_links
		end
		
		assert_equal 5, rval
	end


	def test_sentence_parsed_p_should_return_false_for_unparsed_sentences
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.parsed?
		end
		
		assert_equal false, rval
	end
	
	
	def test_sentence_parsed_p_should_return_true_for_parsed_sentences
		rval = nil
		@sentence.parse
		
		assert_nothing_raised do
			rval = @sentence.parsed?
		end
		
		assert_equal true, rval
	end


	def test_sentence_linkages_should_return_linkage_objects
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.linkages
		end
		
		assert_instance_of Array, rval
		rval.each do |obj|
			assert_instance_of LinkParser::Linkage, obj
		end
	end
	

	def test_sentence_length_should_return_number_of_words_in_sentence
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.length
		end
		
		assert_equal 6, rval
	end


	def test_sentence_word_returns_tokenized_word
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.word( 0 )
		end
		
		assert_equal "LEFT-WALL", rval
	end
	
	def test_sentence_bracketbracket_returns_tokenized_word
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence[ -1 ]
		end
		
		assert_equal "RIGHT-WALL", rval
	end
	
	def test_sentence_null_count_should_return_count_of_null_links
		sent = @dict.parse( "This sentence contains a gravel superfluous word.")
		rval = nil
		
		assert_nothing_raised do
			rval = sent.null_count
		end
		
		rval = 1
	end

end

