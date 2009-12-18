#!/usr/bin/ruby

require 'linkparser_ext'


# The LinkParser top-level namespace.
#  
# == Authors
#  
#  * Michael Granger <ged@FaerieMUD.org>
#  * Martin Chase <stillflame@FaerieMUD.org>
#  
# == Version
# 
#   $Id$
#  
# == License
# 
# :include: LICENSE
#--
#  
# See the LICENSE file for copyright/licensing information.
module LinkParser

	require 'linkparser/sentence'
	require 'linkparser/linkage'

	# Release version
	VERSION = '1.0.6'

end # class LinkParser

