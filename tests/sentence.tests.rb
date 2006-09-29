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

	def setup
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
		@sentence = LinkParser::Sentence.new( "The cat runs.", @dict )
	end
	

	def test_sentence_parse_returns_linkage_count
		rval = nil
		
		assert_nothing_raised do
			rval = @sentence.parse
		end
		
		assert_equal rval, 1, "'The cat runs.' should only have one linkage"
	end


	def test_sentence_length_should_return_word_count
		rval = nil
		assert_nothing_raised do
			rval = @sentence.length
		end
		assert_equal 6, rval, "'The cat runs.' should have a length of 6"
	end

	
	# sentence_parse(Sentence sent, Parse_Options opts);
	# sentence_length(Sentence sent);
	# sentence_get_word(Sentence sent, int wordnum);
	# sentence_null_count(Sentence sent);
	# sentence_num_linkages_found(Sentence sent);
	# sentence_num_valid_linkages(Sentence sent);
	# sentence_num_linkages_post_processed(Sentence sent);
	# sentence_num_violations(Sentence sent, int i);
	# sentence_disjunct_cost(Sentence sent, int i);
	# 
	# sentence_get_nth_word(Sentence sent, int i);
	# sentence_nth_word_has_disjunction(Sentence sent, int i);

end

