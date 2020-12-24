# -*- ruby -*-
# frozen_string_literal: true

require 'loggability'

# The LinkParser top-level namespace.
module LinkParser
	extend Loggability


	# Release version
	VERSION = '2.2.0'


	# Loggability API -- set up a logger
	log_as :linkparser


	# Load the correct version if it's a Windows binary gem
	if RUBY_PLATFORM =~/(mswin|mingw)/i
		major_minor = RUBY_VERSION[ /^(\d+\.\d+)/ ] or
			raise "Oops, can't extract the major/minor version from #{RUBY_VERSION.dump}"

		begin
			require "#{major_minor}/linkparser_ext"
		rescue LoadError
			require 'linkparser_ext'
		end
	else
		require 'linkparser_ext'
	end

	require 'linkparser/mixins'
	require 'linkparser/dictionary'
	require 'linkparser/sentence'
	require 'linkparser/linkage'
	require 'linkparser/parseoptions'


end # class LinkParser

