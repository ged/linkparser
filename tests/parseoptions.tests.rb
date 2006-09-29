#!/usr/bin/ruby -w
#
# Unit tests for the LinkParser::ParseOptions class
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

class LinkParser::ParseOptions::TestCase < Test::Unit::TestCase

	Defaults = {
		:verbosity			 => 1,			# Docs say this is 0
		:linkage_limit		 => 100,		# Docs say this is 10000
		:min_null_count		 => 0,
		:max_null_count		 => 0,
		:null_block			 => 1,
		:islands_ok?		 => false,
		:short_length		 => 6,
		#:all_short?			 => false,	# Not in the API
		#:display_short?		 => true,	# Not in the API
		:display_walls?		 => false,
		:display_union?		 => false,
		:allow_null?		 => true,
		:echo_on?			 => false,
		:batch_mode?		 => false,
		:panic_mode?		 => false,
		:screen_width		 => 79,
		:display_on?		 => true,
		:display_postscript? => false,
		:display_bad?		 => false,
		:display_links?		 => false,
	}

	def setup
		@opts = LinkParser::ParseOptions.new
	end


	def test_values_should_all_start_out_with_documented_defaults
		Defaults.each do |key, default|
			rval = nil

			assert_nothing_raised do
				rval = @opts.send( key )
			end
			
			assert_equal default, rval, key
		end
	end

end

