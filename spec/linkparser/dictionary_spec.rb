#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Dictionary class
# $Id$
#
# See the LICENSE file in the distribution for information about copyright and licensing.
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent.parent

	libdir = basedir + 'lib'
	extdir = basedir + 'ext'

	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
}

require 'spec/runner'
require 'linkparser'


describe LinkParser::Dictionary do

	### Work around current system's locale
	before( :all ) do
		$LANG = ENV['LANG']
		ENV['LANG'] = 'en_US.UTF-8'
		$DEBUG = true if ENV['DEBUG']
	end

	after( :all ) do
		ENV['LANG'] = $LANG
	end


	it "can be instantiated using all default values" do
		LinkParser::Dictionary.new.should be_an_instance_of( LinkParser::Dictionary )
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

	describe "instance" do

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

end


