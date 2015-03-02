# -*- ruby -*-
#encoding: utf-8

require_relative '../helpers'

require 'rspec'
require 'linkparser'


describe LinkParser::Sentence do

	before( :all ) do
		@dict = LinkParser::Dictionary.new( 'en', verbosity: 0 )
	end


	let( :dict ) { @dict }
	let( :sentence ) { described_class.new("The cat runs.", dict) }


	it "returns an informational string when inspected before it's been parsed" do
		expect( sentence.inspect ).to match( %r{
			<
				LinkParser::Sentence:0x[[:xdigit:]]+
				\s
				\(unparsed\)
			>
		}x )
	end

	it "returns an informational string when inspected after it's been parsed" do
		sentence.parse
		expect( sentence.inspect ).to match( %r{
			<
				LinkParser::Sentence:0x[[:xdigit:]]+
				\s
				(?-x:"LEFT-WALL the cat\.n runs\.v . RIGHT-WALL")
				/\d+\slinkages
				/\d+\snulls
			>
		}x )
	end

	it "can return itself as a tokenized string" do
		expect( sentence.to_s ).to eq( "LEFT-WALL the cat.n runs.v . RIGHT-WALL" )
	end


	it "returns the linkage count as the result of parsing the sentence" do
		expect( sentence.parse ).to eq( 3 )
	end


	it "accepts parse options when parsing" do
		sentence.parse( verbosity: 0 )
	end


	it "knows how many words are in it, including walls and punctuation" do
		expect( sentence.length ).to eq( 6 )
	end


	it "delegates linkage methods to its first linkage" do
		expect( sentence.num_links ).to eq( 6 )
	end


	it "knows whether or not it's been parsed" do
		expect( sentence ).to_not be_parsed()
		sentence.parse
		expect( sentence ).to be_parsed()
	end


	it "can return its linkages" do
		expect( sentence.linkages.count ).to eq( 3 )
		expect( sentence.linkages ).to all( be_an_instance_of(LinkParser::Linkage) )
	end


	it "can return an Array of all tokenized words" do
		expect( sentence.words ).to eq([
			'LEFT-WALL', 'the', 'cat.n', 'runs.v', '.', 'RIGHT-WALL'
		])
	end


	it "knows that it doesn't have any superfluous words in it" do
		expect( sentence.null_count ).to eq( 0 )
	end



	describe "parsed from a sentence with a superfluous word in it" do

		let( :sentence ) do
			described_class.new("This sentence contains a gravel superfluous word.", dict)
		end


		it "knows how many null links it has" do
			sentence.parse( max_null_count: 3, min_null_count: 1 )
			expect( sentence.null_count ).to eq( 1 )
		end

	end


	describe "parsed from a sentence that yields no linkages" do

		let( :sentence ) { dict.parse("The event that he smiled at me gives me hope") }


		it "raises a descriptive exception if a delegated method is called" do
			expect {
				sentence.diagram
			}.to raise_error( LinkParser::Error, /sentence has no linkages/i )
		end

	end

end

