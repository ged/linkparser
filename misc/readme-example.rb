#!/usr/bin/env irb --prompt-mode xmp

require 'linkparser'

dict = LinkParser::Dictionary.new( :screen_width => 100 )
sent = dict.parse( "People use Ruby for all kinds of nifty things." )
              
sent.subject
sent.verb
sent.object

puts sent.constituent_tree_string

puts sent.diagram
