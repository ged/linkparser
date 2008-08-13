#!/usr/bin/env ruby

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).expand_path.dirname.parent
	
	require basedir + 'loadpath.rb'
	require basedir + 'utils.rb'
}

include UtilityFunctions

require 'linkparser'
require 'text/format'


dict = LinkParser::Dictionary.new( :verbosity => 0 )
s_string = nil
if ARGV.empty?
	s_string = prompt( "Sentence: " ) or exit
else
	s_string = ARGV.join(" ")
end

sentence = dict.parse( s_string )
raise "Failed to parse sentence %p" % [s_string] if
	sentence.linkages.empty?

llength = sentence.linkages.first.links.collect {|l| l.lword.length }.max
rlength = sentence.linkages.first.links.collect {|l| l.rword.length }.max
formatter = Text::Format.new {|f|
	f.body_indent = llength + rlength + 11
	f.first_indent = 0
	f.columns = ENV['COLUMNS'] || 72
}

puts "Llength: %d, rlength: %d" % [ llength, rlength ]

puts ">>> #{sentence}"
sentence.linkages.first.links.each do |link|
	puts '%1$14s: %2$*3$s <-> %4$*5$s %6$s' % [
		colorize( link.label, 'bold green' ),
		colorize( link.lword, 'bold white' ),
		llength + 11,
		colorize( link.rword, 'bold white' ),
		-(rlength + 11),
		formatter.format( link.desc )
	  ]
end


