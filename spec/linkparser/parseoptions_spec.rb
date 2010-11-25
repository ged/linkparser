#!/usr/bin/ruby -w
#
# Specification for the LinkParser::ParseOptions class
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


describe LinkParser::ParseOptions do

	before( :all ) do
		$DEBUG = true if ENV['DEBUG']
	end

	before( :each ) do
		@opts = LinkParser::ParseOptions.new
	end


	it "starts out with documented defaults" do
		@opts.verbosity.should              == 1		# Docs say this is 0
		@opts.linkage_limit.should          == 100		# Docs say this is 10000
		@opts.min_null_count.should         == 0
		@opts.max_null_count.should         == 0
		@opts.null_block.should             == 1
		@opts.islands_ok?.should            == false
		@opts.short_length.should           == 6
		@opts.display_walls?.should         == false
		@opts.display_union?.should         == false
		@opts.allow_null?.should            == true
		@opts.echo_on?.should               == false
		@opts.batch_mode?.should            == false
		@opts.panic_mode?.should            == false
		@opts.screen_width.should           == 79
		@opts.display_on?.should            == true
		@opts.display_postscript?           == false
		@opts.display_bad?.should           == false
		@opts.display_links?.should         == false
		@opts.all_short_connectors?.should  == false
	end

	it "supports all the members mentioned in the documentation" do
		pending "some of them aren't implemented in the link-grammar library" do
			@opts.display_short?.should == true		# Not in the API
		end
	end


	it "knows whether the timer constraints were exceeded or not" do
		@opts.timer_expired?.should == false
	end

	it "knows whether the memory constraints were exceeded or not" do
		@opts.memory_exhausted?.should == false
	end

	it "knows whether the timer constraints were exceeded or not" do
		@opts.resources_exhausted?.should == false
	end

	it "can reset the resource constraints" do
		lambda {
			@opts.reset_resources
		}.should_not raise_error()
	end

	it "knows whether spell_guessing is enabled or not" do
		@opts.spell_guessing_enabled?.should == true
	end

end

