# -*- ruby -*-
#encoding: utf-8

require_relative 'helpers'

require 'rspec'
require 'linkparser'


describe LinkParser do

	before( :all ) do
		@dict = LinkParser::Dictionary.new('en', verbosity: 0)
	end

	let( :sentence ) { @dict.parse( text ) }
	let( :linkage ) { sentence.linkages.first }


	describe 'bugfix for #3: The first linkage for "The cat runs."' do

		let( :text ) { "The cat runs." }


		it "selects cat as the subject" do
			expect( linkage.subject ).to eq( "cat" )
		end

		it "selects runs as the verb" do
			expect( linkage.verb ).to eq( "runs" )
		end

	end

end
