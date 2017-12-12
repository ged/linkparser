# -*- ruby -*-
#encoding: utf-8

require 'linkparser' unless defined?( LinkParser )

class LinkParser::Sentence
	extend Loggability,
	       LinkParser::DeprecationUtilities


	# Use LinkParser's logger
	log_to :linkparser


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

		return "#<%s:%#x %s>" % [
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

	### Proxy method -- auto-delegate calls to the first linkage.
	def method_missing( sym, *args, &block )
		return super unless LinkParser::Linkage.instance_methods.include?( sym )

		linkage_method = LinkParser::Linkage.instance_method( sym )
		meth = lambda do |*args, &block|
			linkage = self.linkages.first or raise LinkParser::Error, "sentence has no linkages"
			linkage_method.bind( linkage ).call( *args, &block )
		end

		self.singleton_class.instance_exec( sym, meth ) do |name, new_method|
			define_method( name, &new_method )
		end

		meth.call( *args, &block )
	rescue => err
		raise err, err.message, err.backtrace[ 0..-2 ]
	end

end # class Sentence

