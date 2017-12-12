# -*- ruby -*-
#encoding: utf-8

require_relative '../helpers'

require 'rspec'
require 'linkparser'


describe LinkParser::Linkage do

	before( :all ) do
		@dict = LinkParser::Dictionary.new( 'en', verbosity: 0 )
	end


	let( :dict ) { @dict }

	let( :text ) { "The flag was wet." }
	let( :sentence ) { @dict.parse(text) }
	let( :linkage ) { sentence.linkages.first }


	it "can build a diagram string for a sentence" do
		expect( linkage.diagram.each_line.to_a ).to include(
			"    +--------------Xp--------------+       \n",
			"    +-------->WV------->+          |       \n",
			"    +-----Wd-----+      |          |       \n",
			"    |      +Ds**c+--Ss--+--Pa--+   +--RW--+\n",
			"    |      |     |      |      |   |      |\n",
			"LEFT-WALL the flag.n was.v-d wet.a . RIGHT-WALL \n",
		)
	end


	it "can build a diagram string for a sentence wrapped to a screen width" do
		diagram = linkage.diagram( max_width: 10 )
		expect( diagram.each_line.map(&:strip).map(&:length) ).to all( be <= 10 )
	end


	it "can build a diagram string without wall-words" do
		expect( linkage.diagram(display_walls: false) ).to_not include( 'RIGHT-WALL' )
	end


	it "can build a 'links and domains' diagram" do
		expect( linkage.links_and_domains.each_line ).to include(
			"       LEFT-WALL      Xp            ----Xp-----  Xp              .\n",
			" (m)   LEFT-WALL      hWV           >---WV---->  dWV             was.v-d\n",
			" (m)   LEFT-WALL      Wd            ----Wd-----  Wd              flag.n\n",
			" (m)   flag.n         Ss            ----Ss-----  Ss              was.v-d\n",
			" (m)   the            D             ----Ds**c--  Ds**c           flag.n\n",
			" (m)   was.v-d        Pa            ----Pa-----  Pa              wet.a\n",
			"       .              RW            ----RW-----  RW              RIGHT-WALL\n",
			"\n"
		)
	end


	it "knows how many words are in the sentence" do
		# LEFT-WALL + words + '.' + RIGHT-WALL = 7
		expect( linkage.num_words ).to eq( 7 )
	end


	it "can return a list of the tokenized words" do
		expect( linkage.words ).to include("LEFT-WALL")
		expect( linkage.words ).to include("the")
		expect( linkage.words ).to include("flag.n")
		expect( linkage.words ).to include("was.v-d")
		expect( linkage.words ).to include("wet.a")
		expect( linkage.words ).to include(".")
		expect( linkage.words ).to include("RIGHT-WALL")
	end


	it "knows how many links are in the sentence" do
		expect( linkage.num_links ).to eq( 7 )
	end


	it "can return the left word for any of its links" do
		 #       LEFT-WALL      Xp      <---Xp---->  Xp        .
		expect( linkage.link_lword(0) ).to eq( linkage.words.index('LEFT-WALL') )
		 # (m)   LEFT-WALL      WV      <---WV---->  WV        was.v-d
		expect( linkage.link_lword(1) ).to eq( linkage.words.index('LEFT-WALL') )
		 # (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
		expect( linkage.link_lword(2) ).to eq( linkage.words.index('LEFT-WALL') )
		 # (m)   flag.n         Ss      <---Ss---->  Ss        was.v-d
		expect( linkage.link_lword(3) ).to eq( linkage.words.index('flag.n') )
		 # (m)   the            D       <---Ds---->  Ds        flag.n
		expect( linkage.link_lword(4) ).to eq( linkage.words.index('the') )
		 # (m)   was.v-d        Pa      <---Pa---->  Pa        wet.a
		expect( linkage.link_lword(5) ).to eq( linkage.words.index('was.v-d') )
		 #       .              RW      <---RW---->  RW        RIGHT-WALL
		expect( linkage.link_lword(6) ).to eq( linkage.words.index('.') )
	end

	it "can return the right word for any of its links" do
		 #       LEFT-WALL      Xp      <---Xp---->  Xp        .
		expect( linkage.link_rword(0) ).to eq( linkage.words.index('.') )
		 # (m)   LEFT-WALL      WV      <---WV---->  WV        was.v-d
		expect( linkage.link_rword(1) ).to eq( linkage.words.index('was.v-d') )
		 # (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
		expect( linkage.link_rword(2) ).to eq( linkage.words.index('flag.n') )
		 # (m)   flag.n         Ss      <---Ss---->  Ss        was.v-d
		expect( linkage.link_rword(3) ).to eq( linkage.words.index('was.v-d') )
		 # (m)   the            D       <---Ds---->  Ds        flag.n
		expect( linkage.link_rword(4) ).to eq( linkage.words.index('flag.n') )
		 # (m)   was.v-d        Pa      <---Pa---->  Pa        wet.a
		expect( linkage.link_rword(5) ).to eq( linkage.words.index('wet.a') )
		 #       .              RW      <---RW---->  RW        RIGHT-WALL
		expect( linkage.link_rword(6) ).to eq( linkage.words.index('RIGHT-WALL') )
	end


	it "can return the length of any of its links" do
		expect( linkage.link_length(0) ).to eq( 5 )
		expect( linkage.link_length(1) ).to eq( 3 )
		expect( linkage.link_length(2) ).to eq( 2 )
		expect( linkage.link_length(3) ).to eq( 1 )
		expect( linkage.link_length(4) ).to eq( 1 )
		expect( linkage.link_length(5) ).to eq( 1 )
		expect( linkage.link_length(6) ).to eq( 1 )

		# Out-of-bounds just returns -1
		expect( linkage.link_length(11) ).to eq( -1 )
	end


	it "can return labels for any of its links" do
		expect( linkage.link_label(0) ).to eq( "Xp" )
		expect( linkage.link_label(1) ).to eq( "WV" )
		expect( linkage.link_label(2) ).to eq( "Wd" )
		expect( linkage.link_label(3) ).to eq( "Ss" )
		expect( linkage.link_label(4) ).to eq( "Ds**c" )
		expect( linkage.link_label(5) ).to eq( "Pa" )
		expect( linkage.link_label(6) ).to eq( "RW" )

		expect( linkage.link_label(7) ).to be_nil
	end


	it "can return left labels for any of its links" do
		expect( linkage.link_llabel(0) ).to eq( "Xp" )
		expect( linkage.link_llabel(1) ).to eq( "hWV" )
		expect( linkage.link_llabel(2) ).to eq( "Wd" )
		expect( linkage.link_llabel(3) ).to eq( "Ss" )
		expect( linkage.link_llabel(4) ).to eq( "D" )
		expect( linkage.link_llabel(5) ).to eq( "Pa" )
		expect( linkage.link_llabel(6) ).to eq( "RW" )

		expect( linkage.link_llabel(7) ).to be_nil
	end


	it "can return labels for any of its links" do
		expect( linkage.link_rlabel(0) ).to eq( "Xp" )
		expect( linkage.link_rlabel(1) ).to eq( "dWV" )
		expect( linkage.link_rlabel(2) ).to eq( "Wd" )
		expect( linkage.link_rlabel(3) ).to eq( "Ss" )
		expect( linkage.link_rlabel(4) ).to eq( "Ds**c" )
		expect( linkage.link_rlabel(5) ).to eq( "Pa" )
		expect( linkage.link_rlabel(6) ).to eq( "RW" )

		expect( linkage.link_rlabel(7) ).to be_nil
	end


	it "can return the number of domains for any link" do
		expect( linkage.link_num_domains(0) ).to eq( 0 )
		expect( linkage.link_num_domains(1) ).to eq( 1 )
		expect( linkage.link_num_domains(2) ).to eq( 1 )
		expect( linkage.link_num_domains(3) ).to eq( 1 )
		expect( linkage.link_num_domains(4) ).to eq( 1 )
		expect( linkage.link_num_domains(5) ).to eq( 1 )
		expect( linkage.link_num_domains(6) ).to eq( 0 )

		expect( linkage.link_num_domains(112) ).to eq( -1 )
	end


	it "can return the names of the domains of any of its links" do
		expect( linkage.link_domain_names(0) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(0) ).to be_empty

		expect( linkage.link_domain_names(1) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(1) ).to eq( ['m'] )

		expect( linkage.link_domain_names(2) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(2) ).to eq( ['m'] )

		expect( linkage.link_domain_names(3) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(3) ).to eq( ["m"] )

		expect( linkage.link_domain_names(4) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(4) ).to eq( ['m'] )

		expect( linkage.link_domain_names(5) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(5) ).to eq( ['m'] )

		expect( linkage.link_domain_names(6) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(6) ).to be_empty

		expect( linkage.link_domain_names(12) ).to be_an_instance_of( Array )
		expect( linkage.link_domain_names(12) ).to be_empty
	end


	it "can return the disjunct strings for any of its words" do
		expect( linkage.disjunct_strings.length ).to eq( linkage.num_words )
	end


	it "can return parsed disjuncts for any of its words" do
		expect( linkage.disjuncts.length ).to eq( linkage.num_words )
	end


	it "can report on the various cost metrics of the parse" do
		expect( linkage.unused_word_cost ).to be_an_instance_of( Integer )
		expect( linkage.disjunct_cost ).to be_an_instance_of( Integer )
		expect( linkage.link_cost ).to be_an_instance_of( Integer )
	end


	### :FIXME: I don't know what these do/mean yet, so for now just test to
	### make sure they're implemented. They should really be tested with
	### sentences that have predictable results.
	it "implements Link Grammar predicate methods" do
		expect( linkage ).to respond_to( :violation_name )
	end


	 #       LEFT-WALL      Xp      <---Xp---->  Xp        .
	 # (m)   LEFT-WALL      WV      <---WV---->  WV        was.v-d
	 # (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
	 # (m)   flag.n         Ss      <---Ss---->  Ss        was.v-d
	 # (m)   the            D       <---Ds---->  Ds        flag.n
	 # (m)   was.v-d        Pa      <---Pa---->  Pa        wet.a
	 #       .              RW      <---RW---->  RW        RIGHT-WALL
	it "contains link structs describing the linkage" do
		expect( linkage.links ).to be_an_instance_of( Array )
		expect( linkage.links.length ).to eq( 7 )

		linkage.links.each do |link|
			expect( link ).to be_a_kind_of( Struct )
		end

		expect( linkage.links.first.lword ).to eq( 'LEFT-WALL' )
		expect( linkage.links.first.label ).to eq( 'Xp' )
		expect( linkage.links.last.rword ).to eq( 'RIGHT-WALL' )
		expect( linkage.links.last.label ).to eq( 'RW' )
		expect( linkage.links[3].lword ).to eq( 'flag.n' )
		expect( linkage.links[3].rword ).to eq( 'was.v-d' )
		expect( linkage.links[3].label ).to eq( 'Ss' )
	end


	it "knows what word is the verb in the sentence" do
		expect( linkage.verb ).to eq( "was" )
	end


	it "can return the verb without stripping the subscript" do
		expect( linkage.verb(keep_subscript: true) ).to eq( "was.v-d" )
	end


	it "knows what word is the subject of the sentence" do
		expect( linkage.subject ).to eq( "flag" )
	end


	it "can return the subject without stripping the subscript" do
		expect( linkage.subject(keep_subscript: true) ).to eq( "flag.n" )
	end


	context "for sentences with a direct object" do

		let( :text ) { "The dog fetches the ball." }


		it "knows what word is the object of the sentence" do
			expect( linkage.object ).to eq( "ball" )
		end


		it "can return the object without stripping the subscript" do
			expect( linkage.object(keep_subscript: true) ).to eq( linkage.words[5] )
		end

	end


	it "knows when the sentence doesn't have a direct object" do
		expect( linkage.object ).to be_nil()
	end


	it "knows which of its words are nouns" do
		expect( linkage.nouns.size ).to eq( 1 )
		expect( linkage.nouns ).to include( "flag" )
	end


	it "returns an informational string when inspected" do
		expect( linkage.inspect ).to match( /Linkage:0x[[:xdigit:]]+: \[\d+ links\]/ )
	end


	context "from a simple sentence with a direct object" do

		let( :text ) { "The dog fetches the ball." }


		it "knows what word is the object in the sentence" do
			#     +------------------Xp------------------+
			#     +-------->WV------->+                  |
			#     +-----Wd----+       +------Ou-----+    |
			#     |      +Ds**+---Ss--+      +--Dmu-+    +--RW--+
			#     |      |    |       |      |      |    |      |
			# LEFT-WALL the dog.n fetches.v the ball.n-u . RIGHT-WALL
			expect( sentence.object ).to eq( 'ball' )

		end

	end


	it "should know that it's not an imperative sentence" do
		expect( linkage.imperative? ).to be_falsey()
	end


	context "from an imperative sentence" do

		let( :text ) { "Go to the store!" }


		it "knows that it's an imperative sentence" do
			expect( linkage.imperative? ).to be_truthy()
		end


	end


	context "bugfixes" do

		it "also strips off the '.p' from the subject and object when they are plural" do
			sent = dict.parse( 'People like goats.' )
			expect( sent.subject ).to_not match( /people\.p/i )
			expect( sent.object ).to_not match( /goats\.p/i )
		end

	end

end
