#!/usr/bin/ruby -w
#
# Specification for various bugfixes to the LinkParser binding
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

require 'spec/runner'
require 'linkparser'

# @dict = LinkParser::Dictionary.new( :verbosity => 0 )
# s = LinkParser::Sentence.new('The cat runs.',@dict)
# puts s.linkages.first.verb  #  "cat.n" !?!?!
describe %{bugfix for #3: The first linkage for "The cat runs."} do
	before( :each ) do
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
		@sentence = @dict.parse( "The cat runs." )
		@linkage = @sentence.linkages.first
	end
	

	it "thinks cat is the subject" do
		@linkage.subject.should == "cat"
	end

	it "thinks runs is the verb" do
		@linkage.verb.should == "runs"
	end
end

