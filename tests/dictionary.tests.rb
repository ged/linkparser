#!/usr/bin/ruby -w
#
# Unit tests for the LinkParser::Dictionary class
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

class LinkParser::Dictionary::TestCase < Test::Unit::TestCase

	def test_new_with_no_language_should_create_english_dict
		dict = nil
		assert_nothing_raised do
			dict = LinkParser::Dictionary.new
		end
		
		assert_instance_of LinkParser::Dictionary, dict
	end
	
	def test_new_with_only_opthash_should_create_english_dict
		dict = nil
		assert_nothing_raised do
			dict = LinkParser::Dictionary.new( :verbosity => 2 )
		end
		
		assert_instance_of LinkParser::Dictionary, dict
	end
	
	def test_new_with_bad_number_of_args_should_raise_an_error
		assert_raises( ArgumentError ) do
			LinkParser::Dictionary.new( "foo", "bar", "baz" )
		end
	end
	
	def test_new_with_language_argument_should_create_dict_with_language
		dict = nil
		assert_nothing_raised do
			dict = LinkParser::Dictionary.new( "en" )
		end
		
		assert_instance_of LinkParser::Dictionary, dict
	end


	def test_new_with_language_argument_and_opthash_should_create_dict_with_language
		dict = nil
		assert_nothing_raised do
			dict = LinkParser::Dictionary.new( "en", :verbosity => 2 )
		end
		
		assert_instance_of LinkParser::Dictionary, dict
	end


	def test_new_with_unknown_dictionary_should_raise_an_error
		assert_raises( LinkParser::Error ) do
			LinkParser::Dictionary.new( "foo", "bar", "baz", "bim" )
		end
	end


	def test_new_with_unknown_language_should_raise_an_error
		assert_raises( LinkParser::Error ) do
			LinkParser::Dictionary.new( "zz" )
		end
	end


	def test_cost_is_not_zero
		dict = nil
		assert_nothing_raised do
			dict = LinkParser::Dictionary.new
		end
		
		assert_equal 1000, dict.max_cost
	end


	def test_parse_returns_sentence_object
		dict = LinkParser::Dictionary.new
		rval = nil
		
		assert_nothing_raised do
			rval = dict.parse( "The dog plays with the ball." )
		end
		
		assert_instance_of LinkParser::Sentence, rval
	end
	
	
	def test_optpairs_passed_to_new_should_show_up_in_options
		dict = nil
		assert_nothing_raised do
			dict = LinkParser::Dictionary.new( :verbosity => 3 )
		end
		
		assert_instance_of Hash, dict.options
		assert_equal 3, dict.options[:verbosity]
	end

end

