#!/usr/bin/ruby

require 'linkparser' unless defined?( LinkParser )

# 
# A Sentence is the API's representation of an input string, tokenized
# and interpreted according to a specific Dictionary. After a Sentence
# is created and parsed, various attributes of the resulting set of
# linkages can be obtained.
# 
# == Authors
# 
# * Michael Granger <ged@FaerieMUD.org>
# 
# == Version
#
#  $Id: sentence.rb,v 23a39531870a 2011/01/11 18:18:12 ged $
# 
# == License
# 
#  :include: LICENSE
#--
#  
# See the LICENSE file for copyright/licensing information.
class LinkParser::Sentence

	######
	public
	######

	### Return a human-readable representation of the Sentence object.
	def inspect
		contents = ''
		if self.parsed?
			contents = %{"%s"/%d linkages/%d nulls} % [
				self.to_s,
				self.num_linkages_found,
				self.null_count,
			]
		else
			contents = "(unparsed)"
		end

		return "#<%s:0x%x %s>" % [
			self.class.name,
			self.object_id / 2,
			contents,
		]
	end


	### Print out the sentence
	def to_s
		return self.words.join(" ")
	end


	#########
	protected
	#########

	### Return the singleton class for this object
	def singleton_class
		class << self; self; end
	end


	### Proxy method -- auto-delegate calls to the first linkage.
	def method_missing( sym, *args )

		# Check both symbol and string for forward-compatibility with 1.9.x
		return super unless
			LinkParser::Linkage.instance_methods.include?( sym.to_s ) ||
			LinkParser::Linkage.instance_methods.include?( sym )

		linkage = self.linkages.first or raise LinkParser::Error, "sentence has no linkages"

		meth = linkage.method( sym )
		self.singleton_class.send( :define_method, sym, &meth )

		meth.call( *args )
	rescue => err
		raise err, err.message, err.backtrace[ 0..-2 ]
	end

end # class Sentence

