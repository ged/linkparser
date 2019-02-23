# -*- ruby -*-
# frozen_string_literal: true

require 'linkparser' unless defined?( LinkParser )

class LinkParser::Dictionary
	extend Loggability

	# Use LinkParser's logger
	log_to :linkparser

end # class LinkParser::Dictionary

