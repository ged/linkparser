#!/usr/bin/ruby -w
#
# Unit tests for the LinkParser::ParseOptions class
# $Id$
#
# Copyright (c) 2007 The FaerieMUD Consortium
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
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

