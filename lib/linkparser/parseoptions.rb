# -*- ruby -*-
# frozen_string_literal: true

require 'linkparser' unless defined?( LinkParser )


# LinkParser parse options class. Instances of this class are used to specify the different
# parameters that are used to parse sentences. Examples of the kinds of things that are
# controlled by ParseOptions include maximum parsing time and memory, whether to use
# null-links, and whether or not to use "panic" mode. This data structure is passed in to
# the various parsing and printing routines along with the sentence.
class LinkParser::ParseOptions
	extend Loggability,
	       LinkParser::DeprecationUtilities

	# Use LinkParser's logger
	log_to :linkparser


	### Return an Array of valid option names as Symbols.
	def self::option_names
		return @option_names ||= instance_methods( false ).
			grep( /^[a-z].*=$/ ).map {|sym| sym.to_s.chomp('=').to_sym }
	end


	### Return a new LinkParser::ParseOptions with the values of the receiver merged with
	### those from the +other+ object.
	def merge( other )
		new_options = self.dup
		new_options.merge!( other )
		return new_options
	end


	### Overwrite the option settings on the receiver with those from the +other+
	### object.
	def merge!( other )
		other.to_hash.each do |key, val|
			self.send( "#{key}=", val )
		end
	end


	### Return the options as a Hash.
	def to_hash
		return self.class.option_names.each_with_object( {} ) do |optname, accum|
			val = if self.respond_to?( "#{optname}?" )
					self.send( "#{optname}?" )
				else
					self.send( optname )
				end

			accum[ optname ] = val
		end
	end

end # class LinkParser::ParseOptions
