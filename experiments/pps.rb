#!/usr/bin/ruby

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent
	require basedir + 'utils.rb'

	$LOAD_PATH.unshift( basedir + 'lib' )
	$LOAD_PATH.unshift( basedir + 'ext' )

	include UtilityFunctions
}

require 'linkparser'

dict = LinkParser::Dictionary.new( :screen_width => 120 )
loop do
	input_string = prompt( "Sentence to parse: " )
	break if input_string.empty?

	sent = dict.parse( input_string.chomp )

	sent.linkages.each do |linkage|
		message "%s <%s> %s\n" % [ linkage.subject, linkage.verb, linkage.object ]
	end
end

