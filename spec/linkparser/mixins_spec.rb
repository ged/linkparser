#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Sentence class
# $Id: mixins_spec.rb,v 54e4e2ff8899 2010/11/25 00:50:55 ged $
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
require 'linkparser/mixins'


describe LinkParser, "mixins" do

	describe LinkParser::DeprecationUtilities do

		before( :all ) do
			@class = Class.new do
				extend LinkParser::DeprecationUtilities
				def no_args
					return :no_args_result
				end
				def two_args( arg1, arg2 )
					return [arg1, arg2]
				end
				def splat_args( *args )
					return args
				end

				deprecated_method :no_args
				deprecated_method :two_args, :splat_args
			end
			@object = @class.new
		end

		it "provides a function for marking a method as deprecated" do
			@object.should_receive( :warn ).with( /deprecated/i )
			@object.no_args.should == :no_args_result
		end

		it "provides a function for marking a method that takes arguments as deprecated" do
			@object.should_receive( :warn ).with( /deprecated/i )
			@object.two_args( :arg_one, :arg_two ).should == [ :arg_one, :arg_two ]
		end

		it "provides a function for marking a method with splatted arguments as deprecated" do
			@object.should_receive( :warn ).with( /deprecated/i )
			@object.splat_args( :arg_one, :arg_two ).should == [ :arg_one, :arg_two ]
		end


	end

end

