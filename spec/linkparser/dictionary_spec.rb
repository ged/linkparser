# -*- ruby -*-
# frozen_string_literal: true

require_relative '../helpers'

require 'rspec'
require 'linkparser'


describe LinkParser::Dictionary do

	### Tests expect English locale
	before( :all ) do
		$LANG = ENV['LANG']
		ENV['LANG'] = 'en_US.UTF-8'
		$DEBUG = true if ENV['DEBUG']
	end

	after( :all ) do
		ENV['LANG'] = $LANG
	end


	it "can be instantiated using all default values" do
		expect( LinkParser::Dictionary.new ).to be_an_instance_of( LinkParser::Dictionary )
	end

	it "can be instantiated with an options hash" do
		dict = LinkParser::Dictionary.new( :verbosity => 2 )
		expect( dict ).to be_a( LinkParser::Dictionary )
		expect( dict.options[:verbosity] ).to eq( 2 )
	end

	it "raises an error when created with an bad number of arguments" do
		expect {
			LinkParser::Dictionary.new( "foo", "bar", "baz" )
		}.to raise_error( ArgumentError )
	end

	it "can be instantiated with a language argument" do
		dict = LinkParser::Dictionary.new( 'en' )
		expect( dict ).to be_a( LinkParser::Dictionary )
	end

	it "can be instantiated with both a language and an options hash" do
		dict = LinkParser::Dictionary.new( 'en', :verbosity => 2 )
		expect( dict.options[:verbosity] ).to eq( 2 )
	end

	it "raises an exception if created with an unknown language" do
		expect {
			LinkParser::Dictionary.new( 'ie' )
		}.to raise_error( LinkParser::Error )
	end

	it "raises an exception if created with an unknown language" do
		expect {
			LinkParser::Dictionary.new('zz')
		}.to raise_error( LinkParser::Error )
	end


	context "instance" do

		TEST_SENTENCE = "The dog plays with the ball."

		before( :all ) do
			@dict = LinkParser::Dictionary.new( verbosity: 0, max_null_count: 18, islands_ok: true )
		end


		it "can parse a sentence" do
			sentence = @dict.parse( TEST_SENTENCE )
			expect( sentence ).to be_an_instance_of( LinkParser::Sentence )
		end

		it "passes on its options to the sentences it parses" do
			sentence = @dict.parse( TEST_SENTENCE )
			expect( sentence.options.max_null_count ).to eq( 18 )
			expect( sentence.options.verbosity ).to eq( 0 )
			expect( sentence.options.islands_ok? ).to eq( true )
		end
	end

end


