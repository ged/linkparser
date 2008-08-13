#!/usr/bin/ruby -w
#
# Specification for the LinkParser::ParseOptions class
# $Id$
#
# Copyright (c) 2007, 2008 The FaerieMUD Consortium
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
	require basedir + 'loadpath.rb'
}

require 'spec/runner'
require 'linkparser'


describe LinkParser::ParseOptions do

	before( :each ) do
		@opts = LinkParser::ParseOptions.new
	end
	

	it "starts out with documented defaults" do
		@opts.verbosity.should      == 1		# Docs say this is 0
		@opts.linkage_limit.should  == 100		# Docs say this is 10000
		@opts.min_null_count.should == 0
		@opts.max_null_count.should == 0
		@opts.null_block.should     == 1
		@opts.islands_ok?.should    == false
		@opts.short_length.should   == 6
		@opts.display_walls?.should == false
		@opts.display_union?.should == false
		@opts.allow_null?.should    == true
		@opts.echo_on?.should       == false
		@opts.batch_mode?.should    == false
		@opts.panic_mode?.should    == false
		@opts.screen_width.should   == 79
		@opts.display_on?.should    == true
		@opts.display_postscript?   == false
		@opts.display_bad?.should   == false
		@opts.display_links?.should == false
	end

	it "supports all the members mentioned in the documentation" do
		pending "some of them aren't implemented in the link-grammar library"
		@opts.all_short?.should     == false	# Not in the API
		@opts.display_short?.should == true		# Not in the API
	end

end

