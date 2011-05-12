#!/usr/bin/ruby -w
#
# Specification for various bugfixes to the LinkParser binding
# $Id: bugfixes_spec.rb,v 1eddd00723e6 2010/11/22 15:59:36 ged $
#
# See the LICENSE file in the distribution for information about copyright and licensing.
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent

	libdir = basedir + 'lib'
	extdir = basedir + 'ext'

	$LOAD_PATH.unshift( basedir.to_s ) unless $LOAD_PATH.include?( basedir.to_s )
	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
}

require 'rspec'

require 'linkparser'

# @dict = LinkParser::Dictionary.new( :verbosity => 0 )
# s = LinkParser::Sentence.new('The cat runs.',@dict)
# puts s.linkages.first.verb  #  "cat.n" !?!?!
describe %{bugfix for #3: The first linkage for "The cat runs."} do
	before( :all ) do
		$DEBUG = true if ENV['DEBUG']
	end

	before( :each ) do
		@dict = LinkParser::Dictionary.new( 'en', :verbosity => 0 )
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

