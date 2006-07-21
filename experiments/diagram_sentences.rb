#!/usr/bin/ruby

$LOAD_PATH.unshift "ext"

require 'linkparser'
require './utils.rb'

include UtilityFunctions

dict = LinkParser::Dictionary.new( :screen_width => 120 )
loop do
	input_string = prompt( "Sentence to diagram: " )
	break if input_string.empty?

	sent = dict.parse( input_string.chomp )

	sent.linkages.each do |linkage|
		puts linkage.diagram, linkage.links_and_domains
	end
end


### Example output:
# Sentence to diagram:  The bat left the cave just after sunset.
# 
#     +---------------------------Xp--------------------------+
#     +-----Wd----+      +----Os----+                         |
#     |      +-Ds-+--Ss--+    +--Ds-+--Ma--+-MVp-+---Jp--+    |
#     |      |    |      |    |     |      |     |       |    |
# LEFT-WALL the bat.n left.v the cave.n just.a after sunset.n . 
# 
#            LEFT-WALL      Xp      <---Xp---->  Xp        .
#  (m)       LEFT-WALL      Wd      <---Wd---->  Wd        bat.n
#  (m)       the            D       <---Ds---->  Ds        bat.n
#  (m)       bat.n          Ss      <---Ss---->  S         left.v
#  (m)       left.v         O       <---Os---->  Os        cave.n
#  (m)       the            D       <---Ds---->  Ds        cave.n
#  (m) (e)   cave.n         M       <---Ma---->  Ma        just.a
#  (m) (e)   just.a         MV      <---MVp--->  MVp       after
#  (m) (e)   after          J       <---Jp---->  Jp        sunset.n
#            .              RW      <---RW---->  RW        RIGHT-WALL
# 
