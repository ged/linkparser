# -*- ruby -*-
# frozen_string_literal: true

require_relative '../helpers'

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
			expect( @object ).to receive( :warn ).with( /deprecated/i )
			expect( @object.no_args ).to eq( :no_args_result )
		end

		it "provides a function for marking a method that takes arguments as deprecated" do
			expect( @object ).to receive( :warn ).with( /deprecated/i )
			expect( @object.two_args( :arg_one, :arg_two ) ).to eq( [ :arg_one, :arg_two ] )
		end

		it "provides a function for marking a method with splatted arguments as deprecated" do
			expect( @object ).to receive( :warn ).with( /deprecated/i )
			expect( @object.splat_args( :arg_one, :arg_two ) ).to eq( [ :arg_one, :arg_two ] )
		end


	end

end

