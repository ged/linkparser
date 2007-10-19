#!/usr/bin/ruby
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
# == License
#
# Copyright (c) 2007 The FaerieMUD Consortium
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# == Version
#
#  $Id$
# 

require 'linkparser'


class LinkParser::Sentence

	# SVN Revision
	SVNRev = %q$Rev$

	# SVN Id
	SVNId = %q$Id$


	######
	public
	######

	### Return a human-readable representation of the Sentence object.
	def inspect
		%{#<%s:0x%x "%s"/%d linkages/%d nulls>} % [
			self.class.name,
			self.object_id / 2,
			self.words.join(" "),
			self.num_linkages_found,
			self.null_count,
		]
	end


	### Return the Array of words in the sentence as tokenized by the
	### parser.
	def words
		(0...self.length).to_a.collect do |i|
			self.word( i )
		end
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
		linkage = self.linkages.first
		return super unless !linkage.nil? && linkage.respond_to?( sym )
		
		meth = linkage.method( sym )
		self.singleton_class.send( :define_method, sym, &meth )
		
		meth.call( *args )
	rescue => err
		raise err, err.message, err.backtrace[ 0..-2 ]
	end

end # class Sentence

