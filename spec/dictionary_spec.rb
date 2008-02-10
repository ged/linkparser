#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Dictionary class
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
	require basedir + "linkparser-path.rb"
}

require 'spec/runner'
require 'linkparser'


describe LinkParser::Dictionary do
	it "can be instantiated using all default values" do
		lambda { LinkParser::Dictionary.new }.should_not raise_error()
	end

	it "can be instantiated with an options hash" do
		LinkParser::Dictionary.new( :verbosity => 2 ).options[:verbosity].should == 2
	end

	it "raises an error when created with an bad number of arguments" do
		lambda {
			LinkParser::Dictionary.new( "foo", "bar", "baz" )
		}.should raise_error(ArgumentError)
	end

	it "can be instantiated with a language argument" do
		lambda {LinkParser::Dictionary.new( 'en' )}.should_not raise_error()
	end

	it "can be instantiated with both a language and an options hash" do
		LinkParser::Dictionary.new('en', :verbosity => 2).options[:verbosity].should == 2
	end

	it "raises an exception if created with unknown dictionaries" do
		lambda {
			LinkParser::Dictionary.new('foo', 'bar', 'baz', 'bim')
		}.should raise_error( LinkParser::Error )
	end

	it "raises an exception if created with an unknown language" do
		lambda {
			LinkParser::Dictionary.new('zz')
		}.should raise_error( LinkParser::Error )
	end

end

describe "An instance of LinkParser::Dictionary" do

	TEST_SENTENCE = "The dog plays with the ball."

	before( :each ) do
		@dict = LinkParser::Dictionary.new( 
			:verbosity => 0,
			:max_null_count => 18,
			:echo_on => true
		  )
	end
	

	it "knows what the total cost of its linkages are" do
		@dict.max_cost.should be_an_instance_of(Fixnum)
	end

	it "can parse a sentence" do
		@dict.parse( TEST_SENTENCE ).
			should be_an_instance_of( LinkParser::Sentence )
	end
	
	it "passes on its options to the sentences it parses" do
		sentence = @dict.parse( TEST_SENTENCE )
		sentence.options.max_null_count.should == 18
		sentence.options.verbosity.should == 0
		sentence.options.echo_on?.should == true
	end
end

