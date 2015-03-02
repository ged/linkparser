# -*- ruby -*-
#encoding: utf-8

# SimpleCov test coverage reporting; enable this using the :coverage rake task
require 'simplecov' if ENV['COVERAGE']
require 'rspec'

require 'loggability/spechelpers'

require 'linkparser'


$DEBUG = true if ENV['DEBUG']


### RSpec helper functions.
module LinkParser::SpecHelpers
end


### Mock with RSpec
RSpec.configure do |config|
	config.run_all_when_everything_filtered = true
	config.filter_run :focus
	config.order = 'random'
	config.mock_with( :rspec ) do |mock|
		mock.syntax = :expect
	end

	config.include( Loggability::SpecHelpers )
	config.include( LinkParser::SpecHelpers )
end

# vim: set nosta noet ts=4 sw=4:

