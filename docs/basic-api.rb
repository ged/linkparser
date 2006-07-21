#!/usr/bin/ruby 

# The C equivalent:
#   opts  = parse_options_create();
#   dict  = dictionary_create("4.0.dict", "4.0.knowledge", NULL, "4.0.affix");
#   sent = sentence_create(input_string[i], dict);
#   num_linkages = sentence_parse(sent, opts);
#   linkage = linkage_create(0, sent, opts);
#   printf("%s\n", diagram = linkage_print_diagram(linkage));
#   string_delete(diagram);
#   linkage_delete(linkage);
#   dictionary_delete(dict);
#   parse_options_delete(opts);


opts = {
	:verbosity					=> 0,
	:linkage_limit				=> 10000,
	:min_null_count				=> 0,
	:max_null_count				=> 0,
	:null_block					=> 1,
	:islands_ok					=> false,
	:short_length				=> 6,
	:all_short					=> false,
	:display_short				=> true,
	:display_word_subscripts 	=> true,
	:display_link_subscripts 	=> true,
	:display_walls				=> false,
	:display_union				=> false,
	:allow_null					=> true,
	:echo_on					=> false,
	:batch_mode					=> false,
	:panic_mode					=> false,
	:screen_width				=> 79,
	:display_on					=> true,
	:display_postscript			=> false,
	:display_bad				=> false,
	:display_links				=> false,
}

paths = [
	dict_name,
	pp_name,
	cons_name,
	affix_name
]

dict = LinkParser::Dict.create( opts )
dict = LinkParser::Dict.create( paths )
dict = LinkParser::Dict.create( "en" )
dict = LinkParser::Dict.create( "en", opts )

sentence = dict.parse( input_string, :verbosity => 3, :islands_ok => true )
# -or-
sentence = LinkParser::Sentence.new( input_string, dict )
linkage_count = sentence.parse( :verbosity => 3, :islands_ok => true )

sentence.linkages.each do |linkage|
	puts linkage.diagram
end
sentence.each_linkage do |linkage|
	puts linkage.diagram
end


