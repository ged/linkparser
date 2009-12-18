#!/usr/bin/env ruby

require 'pathname'

basedir = Pathname(__FILE__).dirname.parent

$LOAD_PATH.unshift( basedir + 'lib' )
$LOAD_PATH.unshift( basedir + 'ext' )

require 'linkparser'

parser = LinkParser::Dictionary.new( :verbosity => 0, :max_parse_time => 1 )
sentence = parser.parse( "The dog ran home." )

sentence.linkages.each do |linkage|
	puts linkage.diagram
end

p sentence.links.map {|link| link.rlabel[0] }

