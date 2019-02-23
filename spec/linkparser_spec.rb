# -*- ruby -*-
# frozen_string_literal: true

require_relative 'helpers'

require 'rspec'
require 'linkparser'


describe LinkParser do

	it "knows what version of the link-grammar library it was built against" do
		expect( LinkParser.link_grammar_version ).to match( /link-grammar-\d+\.\d+\.\d+/i )
	end


	it "knows what the configuration of the link-grammar library was" do
		expect( LinkParser.link_grammar_config ).to match( /compiled with:/i )
	end

end

