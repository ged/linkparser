# -*- ruby -*-
#encoding: utf-8

require_relative 'helpers'

require 'rspec'
require 'linkparser'


describe LinkParser do

	it "knows what version of the link-grammar library it was built against" do
		expect( LinkParser.link_grammar_version ).to match( /link-grammar-\d+\.\d+\.\d+/i )
	end

end

