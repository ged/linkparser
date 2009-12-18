#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Linkage class
# $Id$
#
# See the LICENSE file in the distribution for information about copyright and licensing.
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent.parent

	libdir = basedir + 'lib'
	extdir = basedir + 'ext'

	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
}

require 'spec/runner'
require 'linkparser'


describe LinkParser::Linkage do

	before( :all ) do
		@dict = LinkParser::Dictionary.new( 'en', :verbosity => 0 )
	end

	before( :each ) do
		@sentence = @dict.parse( "The flag was wet." )
		@linkage = @sentence.linkages.first
	end


	#     +-------------Xp-------------+
	#     +-----Wd-----+               |
	#     |      +--Ds-+--Ss-+--Pa-+   |
	#     |      |     |     |     |   |
	# LEFT-WALL the flag.n was.v wet.a .
	it "can build a diagram string for a sentence" do
		@linkage.diagram.should =~ /LEFT-WALL/
		@linkage.diagram.should =~ /the/
		@linkage.diagram.should =~ /flag\.n/
		@linkage.diagram.should =~ /was\.v/
		@linkage.diagram.should =~ /wet\.a/

		@linkage.diagram.should =~ /-Xp-/
		@linkage.diagram.should =~ /-Wd-/
		@linkage.diagram.should =~ /-Ds-/
		@linkage.diagram.should =~ /-Ss-/
		@linkage.diagram.should =~ /-Pa-/
	end


	 #       LEFT-WALL      Xp      <---Xp---->  Xp        .
	 # (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
	 # (m)   the            D       <---Ds---->  Ds        flag.n
	 # (m)   flag.n         Ss      <---Ss---->  Ss        was.v
	 # (m)   was.v          Pa      <---Pa---->  Pa        wet.a
	 #       .              RW      <---RW---->  RW        RIGHT-WALL
	it "can build a 'links and domains' diagram" do
		@linkage.links_and_domains.should =~ /LEFT-WALL/
		@linkage.links_and_domains.should =~ /the/
		@linkage.links_and_domains.should =~ /flag\.n/
		@linkage.links_and_domains.should =~ /was\.v/
		@linkage.links_and_domains.should =~ /wet\.a/

		@linkage.links_and_domains.should =~ /-Xp-/
		@linkage.links_and_domains.should =~ /-Wd-/
		@linkage.links_and_domains.should =~ /-Ds-/
		@linkage.links_and_domains.should =~ /-Ss-/
		@linkage.links_and_domains.should =~ /-Pa-/
	end


	it "knows how many words are in the sentence" do
		# LEFT-WALL + words + '.' + RIGHT-WALL = 7
		@linkage.num_words.should == 7
	end


	it "can return a list of the tokenized words" do
		@linkage.words.should include("LEFT-WALL")
		@linkage.words.should include("the")
		@linkage.words.should include("flag.n")
		@linkage.words.should include("was.v-d")
		@linkage.words.should include("wet.a")
		@linkage.words.should include(".")
		@linkage.words.should include("RIGHT-WALL")
	end


	it "knows how many links are in the sentence" do
		@linkage.num_links.should == 6
	end


	it "can return the left word for any of its links" do
	 	#       LEFT-WALL      Xp      <---Xp---->  Xp        .
		@linkage.link_lword( 0 ).should == @linkage.words.index('LEFT-WALL')

		# (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
		@linkage.link_lword( 1 ).should == @linkage.words.index('LEFT-WALL')

		# (m)   the            D       <---Ds---->  Ds        flag.n
		@linkage.link_lword( 2 ).should == @linkage.words.index('the')

		# (m)   flag.n         Ss      <---Ss---->  Ss        was.v
		@linkage.link_lword( 3 ).should == @linkage.words.index('flag.n')

		# (m)   was.v          Pa      <---Pa---->  Pa        wet.a
		@linkage.link_lword( 4 ).should == @linkage.words.index('was.v-d')

		#       .              RW      <---RW---->  RW        RIGHT-WALL
		@linkage.link_lword( 5 ).should == @linkage.words.index('.')

	end

	it "can return the right word for any of its links" do
	 	#       LEFT-WALL      Xp      <---Xp---->  Xp        .
		@linkage.link_rword( 0 ).should == @linkage.words.index('.')

		# (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
		@linkage.link_rword( 1 ).should == @linkage.words.index('flag.n')

		# (m)   the            D       <---Ds---->  Ds        flag.n
		@linkage.link_rword( 2 ).should == @linkage.words.index('flag.n')

		# (m)   flag.n         Ss      <---Ss---->  Ss        was.v
		@linkage.link_rword( 3 ).should == @linkage.words.index('was.v-d')

		# (m)   was.v          Pa      <---Pa---->  Pa        wet.a
		@linkage.link_rword( 4 ).should == @linkage.words.index('wet.a')

		#       .              RW      <---RW---->  RW        RIGHT-WALL
		@linkage.link_rword( 5 ).should == @linkage.words.index('RIGHT-WALL')

	end


	it "can return the length of any of its links" do
		@linkage.link_length( 0 ).should == 5 
		@linkage.link_length( 1 ).should == 2
		@linkage.link_length( 2 ).should == 1
		@linkage.link_length( 3 ).should == 1
		@linkage.link_length( 4 ).should == 1
		@linkage.link_length( 5 ).should == 1

		# Out-of-bounds just returns -1
		@linkage.link_length( 7 ).should == -1
	end


	it "can return labels for any of its links" do
		@linkage.link_label( 0 ).should == "Xp"
		@linkage.link_label( 1 ).should == "Wd"
		@linkage.link_label( 2 ).should == "Ds"
		@linkage.link_label( 3 ).should == "Ss"
		@linkage.link_label( 4 ).should == "Pa"
		@linkage.link_label( 5 ).should == "RW"

		@linkage.link_label( 7 ).should be_nil
	end


	it "can return left labels for any of its links" do
		@linkage.link_llabel( 0 ).should == "Xp"
		@linkage.link_llabel( 1 ).should == "Wd"
		@linkage.link_llabel( 2 ).should == "D"
		@linkage.link_llabel( 3 ).should == "Ss"
		@linkage.link_llabel( 4 ).should == "Pa"
		@linkage.link_llabel( 5 ).should == "RW"

		@linkage.link_llabel( 7 ).should be_nil
	end


	it "can return labels for any of its links" do
		@linkage.link_rlabel( 0 ).should == "Xp"
		@linkage.link_rlabel( 1 ).should == "Wd"
		@linkage.link_rlabel( 2 ).should == "Ds"
		@linkage.link_rlabel( 3 ).should == "Ss"
		@linkage.link_rlabel( 4 ).should == "Pa"
		@linkage.link_rlabel( 5 ).should == "RW"

		@linkage.link_rlabel( 7 ).should be_nil
	end


	it "can return the number of domains for any link" do
		@linkage.link_num_domains( 0 ).should == 0
		1.upto(4) do |i|
			@linkage.link_num_domains( i ).should == 1
		end
		@linkage.link_num_domains( 5 ).should == 0

		@linkage.link_num_domains( 112 ).should == -1
	end


	it "can return the names of the domains of any of its links" do
		@linkage.link_domain_names( 0 ).should be_an_instance_of( Array )
		@linkage.link_domain_names( 0 ).should be_empty

		1.upto(4) do |i|
			@linkage.link_domain_names( i ).should be_an_instance_of( Array )
			@linkage.link_domain_names( i ).should == ["m"]
		end

		@linkage.link_domain_names( 5 ).should be_an_instance_of( Array )
		@linkage.link_domain_names( 5 ).should be_empty

		@linkage.link_domain_names( 12 ).should be_an_instance_of( Array )
		@linkage.link_domain_names( 12 ).should be_empty
	end


	it "can return the disjunct strings for any of its words" do
		@linkage.disjunct_strings.should have( @linkage.num_words ).members
	end


	it "can return parsed disjuncts for any of its words" do
		@linkage.disjuncts.should have( @linkage.num_words ).members
	end


	it "can report on the various cost metrics of the parse" do
		@linkage.unused_word_cost.should be_an_instance_of( Fixnum )
		@linkage.disjunct_cost.should be_an_instance_of( Fixnum )
		@linkage.and_cost.should be_an_instance_of( Fixnum )
		@linkage.link_cost.should be_an_instance_of( Fixnum )
	end


	### :FIXME: I don't know what these do/mean yet, so for now just test to 
	### make sure they're implemented. They should really be tested with
	### sentences that have predictable results.
	it "implements Link Grammar predicate methods" do
		@linkage.should respond_to( :canonical? )
		@linkage.should respond_to( :improper? )
		@linkage.should respond_to( :has_inconsistent_domains? )
		@linkage.should respond_to( :violation_name )
	end


	 #       LEFT-WALL      Xp      <---Xp---->  Xp        .
	 # (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
	 # (m)   the            D       <---Ds---->  Ds        flag.n
	 # (m)   flag.n         Ss      <---Ss---->  Ss        was.v
	 # (m)   was.v          Pa      <---Pa---->  Pa        wet.a
	 #       .              RW      <---RW---->  RW        RIGHT-WALL
	it "contains link structs describing the linkage" do
		@linkage.should have(6).links
		@linkage.links.should be_an_instance_of( Array )

		@linkage.links.each do |link|
			link.should be_a_kind_of( Struct )
		end

		@linkage.links.first.lword.should == 'LEFT-WALL'
		@linkage.links.first.label.should == 'Xp'
		@linkage.links.last.rword.should == 'RIGHT-WALL'
		@linkage.links.last.label.should == 'RW'
		@linkage.links[3].lword.should == 'flag.n'
		@linkage.links[3].rword.should == 'was.v-d'
		@linkage.links[3].label.should == 'Ss'
	end


	it "knows what word is the verb in the sentence" do
		@linkage.verb.should == "was"
	end


	it "knows when the sentence doesn't have a direct object" do
		@linkage.object.should be_nil()
	end


	it "knows which of its words are nouns" do
		@linkage.nouns.should have(1).member
		@linkage.nouns.should include( "flag" )
	end


	MODE1_C_TREE_STRING = "(S (NP The flag)\n   (VP was\n       (ADJP wet))\n   .)\n"
	MODE2_C_TREE_STRING = "[S [NP The flag NP] [VP was [ADJP wet ADJP] VP] . S] \n"
	MODE3_C_TREE_STRING = "(S (NP The flag) (VP was (ADJP wet)) .)\n"

	it "returns an indented sexps for the constituent tree string by default (mode 1)" do
		@linkage.constituent_tree_string.should == MODE1_C_TREE_STRING
	end


	it "returns indented sexps for the constituent tree string if fetched with explicit mode '1'" do
		@linkage.constituent_tree_string( 1 ).should == MODE1_C_TREE_STRING
	end

	it "returns bracketed constituents if constituent tree string is fetched in mode 2" do
		@linkage.constituent_tree_string( 2 ).should == MODE2_C_TREE_STRING
	end

	it "returns unindented sexps for the constituent tree string if constituent tree string " +
	   "is fetched in mode 3" do
		@linkage.constituent_tree_string( 3 ).should == MODE3_C_TREE_STRING
	end

	it "raises an exception for any numeric constituent tree string mode greater than 3" do
		lambda {
			@linkage.constituent_tree_string( 4 )
		}.should raise_error( ArgumentError, /illegal mode 4/i )
	end

	it "raises an exception for any numeric constituent tree string mode less than 1" do
		lambda {
			@linkage.constituent_tree_string( 0 )
		}.should raise_error( ArgumentError, /illegal mode 0/i )
	end


	it "raises an exception when a non-numeric constituent tree string mode is given" do
		lambda {
			@linkage.constituent_tree_string( 'glarg' )
		}.should raise_error( TypeError )
	end

	it "returns an Array of CTree structs for its constituent tree" do
		rval = @linkage.constituent_tree

		rval.should be_an_instance_of( Array )
		rval.should have(1).members
		rval.first.should be_a_kind_of( Struct )
		rval.first.label.should == 'S'
		rval.first.children.should have(3).members
		rval.first.children.collect {|n| n.label }.should include( 'NP', 'VP', '.' )
	end

	it "returns 0 as the number of the current sublinkage since it has no conjunctions" do
	    @linkage.current_sublinkage.should == 0 
	end


	it "returns an informational string when inspected" do
		@linkage.inspect.should =~ /Linkage:0x[[:xdigit:]]+: sublinkage 0: \[\d+ links\]/
	end


	describe "from a simple sentence with a direct object" do
		before( :each ) do
			@sentence = @dict.parse( "The dog ran home." )
			@linkage = @sentence.linkages.first
		end


		it "doesn't have any sublinkages" do
			@linkage.num_sublinkages.should == 1
		end

		it "doesn't change after computing its union" do
			lambda {
				@linkage.compute_union
			}.should_not change( @linkage, :num_sublinkages )
		end


		it "knows what word is the object in the sentence" do
			# This depends on the linkage:
			#       +---------------Xp---------------+
			#       +-----Wd----+                    |
			#       |      +-Ds-+--Ss--+---Ou---+    |
			#       |      |    |      |        |    |
			#   LEFT-WALL the dog.n ran.v-d home.n-u . 
			# ...but it might not be the first one, so check them all.
			@sentence.linkages.find {|linkage| linkage.object == 'home' }

		end

	end


	it "knows that it doesn't have any conjunctions" do
		@linkage.should_not have_conjunction()
	end


	describe "from a sentence with a conjunction" do
		before( :each ) do
			@sentence = 
				@dict.parse( "The ball rolled down the hill and bumped the curb." )
			@linkage = @sentence.linkages.first
		end


		it "knows that it has a conjunction" do
			@linkage.should have_conjunction()
		end

		it "has two sublinkages" do
			@linkage.num_sublinkages.should == 2
		end


		it "adds a sublinkage after computing its union" do
			lambda {
				@linkage.compute_union
			}.should change( @linkage, :num_sublinkages ).from(2).to(3)
		end


		it "knows what word is the verb in the current sublinkage" do
			@linkage.verb.should == 'rolled'
			@linkage.current_sublinkage = 1
			@linkage.verb.should == 'bumped'
		end


		it "knows what word is the object in the current sublinkage" do
			@linkage.object.should == 'hill'
			@linkage.current_sublinkage = 1
			@linkage.object.should == 'curb'
		end

	end


	it "should know that it's not an imperative sentence" do
		@linkage.imperative?.should be_false()
	end


	describe "from an imperative sentence" do
		before( :each ) do
			@sentence = @dict.parse( "Go to the store!" )
			@linkage = @sentence.linkages.first
		end


		it "knows that it's an imperative sentence" do
			@linkage.imperative?.should be_true()
		end


	end


	describe "bugfixes" do

		it "also strips off the '.p' from the subject and object when they are plural" do
			sent = @dict.parse( 'People like goats.' )
			sent.subject.should_not =~ /people\.p/i
			sent.object.should_not =~ /goats\.p/i
		end

	end

end
