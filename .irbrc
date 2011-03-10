# -*- ruby -*-

puts ">>> Adding 'lib' to load path..."
$LOAD_PATH.unshift( "lib", "ext" )

# Try to require the 'linkparser' library
begin
	$stderr.puts "Loading LinkParser..."
	require 'linkparser'
	require 'terminfo'

	cols = TermInfo.screen_columns

    puts "Instantiating a Dictionary as $dict"
    $dict = LinkParser::Dictionary.new( :screen_width => cols - 5 )
rescue => e
	$stderr.puts "Ack! LinkParser failed to load: #{e.message}\n\t" +
		e.backtrace.join( "\n\t" )
end


