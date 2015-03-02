#!/usr/bin/env ruby

require 'nokogiri'
require 'open-uri'
require 'yaml'

LINKTYPE_PAGE = 'http://www.abisource.com/projects/link-grammar/dict/summarize-links.html'

doc = Nokogiri::HTML( open(LINKTYPE_PAGE) )
link_types = doc.css( 'a[@name]' ).each_with_object( {} ) do |node, accum|
	link_type = node['name']
	link_desc = node.next_sibling.text.gsub( /\s{2,}/, ' ' ).strip

	accum[ link_type ] = link_desc
end

puts link_types.to_yaml

