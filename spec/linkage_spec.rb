#!/usr/bin/ruby -w
#
# Specification for the LinkParser::Linkage class
# $Id$
#
# Copyright (c) 2007 The FaerieMUD Consortium
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname.parent
	require basedir + 'linkparser-path.rb'
}

require 'spec/runner'
require 'linkparser'


describe LinkParser::Linkage do

	before( :all ) do
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
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
		@linkage.words.should include("was.v")
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
		@linkage.link_lword( 4 ).should == @linkage.words.index('was.v')

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
		@linkage.link_rword( 3 ).should == @linkage.words.index('was.v')

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
		@linkage.links[3].rword.should == 'was.v'
		@linkage.links[3].label.should == 'Ss'
	end
	

end


class BlahDeBlah

    #1         LEFT-WALL      Xp      <---Xp---->  Xp        .
    #2   (m)   LEFT-WALL      Wd      <---Wd---->  Wd        flag.n
    #3+4 (m)   the            D       <---Ds---->  Ds        flag.n
    #5   (m)   flag.n         Ss      <---Ss---->  Ss        was.v
    #6   (m)   was.v          Pa      <---Pa---->  Pa        wet.a
	def test_linkage_links_should_contain_link_structs_describing_the_linkage
		rval = nil
		
		assert_nothing_raised do
			rval = @linkage.links
		end
		
		assert_instance_of Array, rval
		assert_kind_of Struct, rval.first
		assert_equal 'LEFT-WALL', rval.first.lword, "left word of first link"
		assert_equal 'Xp', rval.first.label, "label of first link"
		assert_equal 'RIGHT-WALL', rval.last.rword, "right word of last link"
		assert_equal 'RW', rval.last.label, "label of last link"
		assert_equal 'flag.n', rval[3].lword, "left word of the fourth link"
		assert_equal 'was.v', rval[3].rword, "right word of the fourth link"
		assert_equal 'Ss', rval[3].label, "label of the fourth link"
	end


	def test_linkage_verb_should_return_sentence_verb
		rval = nil
		
		assert_nothing_raised do
			rval = @linkage.verb
		end
		
		assert_equal "was", rval
	end


	# The ball rolled down the hill and bumped the curb.
	def test_linkage_verb_should_return_vword_of_current_sublinkage_of_conjunction
		rval = nil
		linkage = @conjunct_sentence.linkages.first
		
		assert_nothing_raised do
			rval = linkage.verb
		end
		
		assert_equal 'rolled', rval
		
		assert_nothing_raised do
			linkage.current_sublinkage = 1
			rval = linkage.verb
		end
		
		assert_equal 'bumped', rval
	end

	
	
	Mode1CTreeString = "(S (NP The flag)\n   (VP was\n       (ADJP wet))\n   .)\n"
	Mode2CTreeString = "[S [NP The flag NP] [VP was [ADJP wet ADJP] VP] . S] \n"
	Mode3CTreeString = "(S (NP The flag) (VP was (ADJP wet)) .)\n"
	
	def test_constituent_tree_string_without_mode_should_return_mode1_string
		rval = nil
		
		assert_nothing_raised do
			rval = @linkage.constituent_tree_string
		end
		
		assert_equal Mode1CTreeString, rval
	end


	def test_constituent_tree_string_with_mode1_should_return_mode1_string
		rval = nil
		
		assert_nothing_raised do
			rval = @linkage.constituent_tree_string( 1 )
		end
		
		assert_equal Mode1CTreeString, rval
	end

	def test_constituent_tree_string_with_mode2_should_return_mode2_string
		rval = nil
		
		assert_nothing_raised do
			rval = @linkage.constituent_tree_string( 2 )
		end
		
		assert_equal Mode2CTreeString, rval
	end

	def test_constituent_tree_string_with_mode3_should_return_mode3_string
		rval = nil
		
		assert_nothing_raised do
			rval = @linkage.constituent_tree_string( 3 )
		end
		
		assert_equal Mode3CTreeString, rval
	end

	def test_constituent_tree_string_with_mode4_should_raise_exception
		assert_raises( ArgumentError ) do
			@linkage.constituent_tree_string( 4 )
		end
	end

	def test_constituent_tree_string_with_nonnumeric_should_raise_exception
		assert_raises( TypeError ) do
			@linkage.constituent_tree_string( "Glah" )
		end
	end


	def test_constituent_tree_should_return_array_of_CTree_structs
		rval = nil
		
		assert_nothing_raised do 
			rval = @linkage.constituent_tree
		end
		
		assert_kind_of Array, rval
		assert_kind_of Struct, rval.first
		assert_equal "S", rval.first.label
		assert_equal "NP", rval.first.children.first.label
		assert_equal "The", rval.first.children.first.children.first.label
	end
	
	def test_linkage_current_sublinkage_should_return
		rval = nil
		assert_nothing_raised do
			rval = @linkage.current_sublinkage
		end
		assert_kind_of(Fixnum, rval)
	end

end


describe LinkParser::Linkage, " from a simple sentence" do

	before( :all ) do
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
	end

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

end


describe LinkParser::Linkage, " with a conjunction" do
	before( :all ) do
		@dict = LinkParser::Dictionary.new( :verbosity => 0 )
	end
	
	before( :each ) do
		@sentence = 
			@dict.parse( "The ball rolled down the hill and bumped the curb." )
		@linkage = @sentence.linkages.first
	end


	it "has two sublinkages" do
		@linkage.num_sublinkages.should == 2
	end
	

	it "adds a sublinkage after computing its union" do
		lambda {
			@linkage.compute_union
		}.should change( @linkage, :num_sublinkages ).from(2).to(3)
	end
end


