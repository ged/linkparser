#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Sentence class
# $Id$
#
# See the LICENSE file in the distribution for information about copyright and licensing.
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent.parent

	libdir = basedir + 'lib'
	extdir = basedir + 'ext'

	$LOAD_PATH.unshift( basedir.to_s ) unless $LOAD_PATH.include?( basedir.to_s )
	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
}

require 'rspec'

require 'linkparser'


describe LinkParser::Sentence do

	before( :all ) do
		@dict = LinkParser::Dictionary.new( 'en', :verbosity => 0 )
		$DEBUG = true if ENV['DEBUG']
	end

	before( :each ) do
		@sentence = LinkParser::Sentence.new( "The cat runs.", @dict )
	end


	it "returns an informational string when inspected before it's been parsed" do
		@sentence.inspect.should =~ %r{
			<
				LinkParser::Sentence:0x[[:xdigit:]]+
				\s
				\(unparsed\)
			>
		}x
	end

	it "returns an informational string when inspected after it's been parsed" do
		@sentence.parse
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
		@sentence.should_not be_parsed()
		@sentence.parse
		@sentence.should be_parsed()
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


	describe "parsed from a sentence that yields no linkages" do

		before( :each ) do
			@sentence = @dict.parse( "The event that he smiled at me gives me hope" )
		end

		it "raises a descriptive exception if a delegated method is called" do
			expect {
				@sentence.constituent_tree_string
			}.to raise_error( LinkParser::Error, /sentence has no linkages/i )
		end

	end

end

