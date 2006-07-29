# -*- ruby -*-

puts ">>> Adding 'lib' to load path..."
$LOAD_PATH.unshift( "lib", "ext" )

#puts ">>> Turning on $VERBOSE and $DEBUG..."
# $VERBOSE = $DEBUG = true

require './utils'
include UtilityFunctions

def colored( prompt, *args )
	return ansiCode( *(args.flatten) ) + prompt + ansiCode( 'reset' )
end


# Modify prompt to do highlighting
IRB.conf[:PROMPT][:LINKPARSER] = { # name of prompt mode
 	:PROMPT_I => colored( "%N(%m):%03n:%i>", %w{bold white on_blue} ) + " ",
 	:PROMPT_S => colored( "%N(%m):%03n:%i%l", %w{white on_blue} ) + " ",
 	:PROMPT_C => colored( "%N(%m):%03n:%i*", %w{white on_blue} ) + " ",
    :RETURN => "    ==> %s\n\n"      # format to return value
}
IRB.conf[:PROMPT_MODE] = :LINKPARSER

# Try to require the 'linkparser' library
begin
	puts "Requiring 'linkparser' module..."
	require 'linkparser'
	
	$dict = LinkParser::Dictionary.new
rescue => e
	$stderr.puts "Ack! LinkParser module failed to load: #{e.message}\n\t" +
		e.backtrace.join( "\n\t" )
end

