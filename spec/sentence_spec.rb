#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Sentence class
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
	require basedir + 'linkparser-path.rb'
}

require 'spec/runner'
require 'linkparser'


describe LinkParser::Sentence do


	before( :all ) do
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
	end
	

	before( :each ) do
		@sentence = LinkParser::Sentence.new( "The cat runs.", @dict )
	end
	
	
	it "returns an informational string when inspected" do
		@sentence.inspect.should =~ %r{
			<
				LinkParser::Sentence:0x[[:xdigit:]]+
				\s
				(?-x:"LEFT-WALL the cat runs . RIGHT-WALL")
				/\d+\slinkages
				/\d+\snulls
			>
		}x
	end

	it "can return itself as a tokenized string" do
		@sentence.to_s.should == "LEFT-WALL the cat runs . RIGHT-WALL"
	end
	

	it "returns the linkage count as the result of parsing the sentence" do
		@sentence.parse.should == 1
	end


	it "accepts parse options when parsing" do
		@sentence.parse( :verbosity => 0 )
	end


	it "knows how many words are in it, including walls and punctuation" do
		@sentence.length == 6
	end


	it "delegates linkage methods to its first linkage" do
		@sentence.num_links.should == 5
	end


	it "knows whether or not it's been parsed" do
		@sentence.parsed?.should be_false()
		@sentence.parse
		@sentence.parsed?.should be_true()
	end


	it "can return its linkages" do
		@sentence.should have(1).linkages
		@sentence.linkages.first.should be_an_instance_of( LinkParser::Linkage )
	end

	it "can return words at a specified position" do
		@sentence.word( 0 ).should == 'LEFT-WALL'
		@sentence[ -1 ].should == 'RIGHT-WALL'
	end
	
	it "can return an Array of all tokenized words" do
		@sentence.words.should == [
			'LEFT-WALL', 'the', 'cat', 'runs', '.', 'RIGHT-WALL'
		]
	end
	

	it "knows that it doesn't have any superfluous words in it" do
		@sentence.null_count == 0
	end
	
	

	describe "parsed from a sentence with a superfluous word in it" do

		before( :each ) do
			@sentence = @dict.parse( "This sentence contains a gravel superfluous word.")
		end
	

		it "knows how many null links it has" do
			@sentence.null_count == 1
		end
	end

end

