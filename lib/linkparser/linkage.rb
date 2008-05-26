#!/usr/bin/ruby
# 
# Additional high-level functionality for LinkParser::Sentence objects.
# 
# == Synopsis
# 
#   
# 
# == Authors
# 
# * Michael Granger <ged@FaerieMUD.org>
# 
# == License
#
# Copyright (c) 2007, 2008 The FaerieMUD Consortium
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
# == Version
#
#  $Id$
# 

require 'linkparser'


### Additional high-level functionality for LinkParser::Sentence objects.
class LinkParser::Linkage

	# SVN Revision
	SVNRev = %q$Rev$

	# SVN Id
	SVNId = %q$Id$

	# Descriptions of the linkage types, keyed by linkage symbol
	LinkTypes = {
		:A  => %{connects pre-noun ("attributive") adjectives to following nouns: "The BIG DOG chased me", "The BIG BLACK UGLY DOG chased me".},
		:AA => %{is used in the construction "How [adj] a [noun] was it?". It connects the adjective to the following "a".},
		:AF => %{connects adjectives to verbs in cases where the adjective is fronted, such as questions and indirect questions: "How BIG IS it?"},
		:AL => %{connects a few determiners like "all" or "both" to following determiners: "ALL THE people are here".},
		:AM => %{connects "as" to "much" or "many": "I don't go out AS MUCH now".},
		:AN => %{connects noun-modifiers to following nouns: "The TAX PROPOSAL was rejected".},
		:AZ => %{connects the word "as" back to certain verbs that can take "[obj] as [adj]" as a complement: "He VIEWED him AS stupid".},
		:B  => %{serves various functions involving relative clauses and questions. It connects transitive verbs back to their objects in relative clauses, questions, and indirect questions ("The DOG we CHASED", "WHO did you SEE?"); it also connects the main noun to the finite verb in subject-type relative clauses ("The DOG who CHASED me was black").},
		:BI => %{connects forms of the verb "be" to certain idiomatic expressions: for example, cases like "He IS PRESIDENT of the company".},
		:BT => %{is used with time expressions acting as fronted objects: "How many YEARS did it LAST?".},
		:BW => %{connects "what" to various verbs like "think", which are not really transitive but can connect back to "what" in questions: "WHAT do you THINK?"},
		:C  => %{links conjunctions to subjects of subordinate clauses ("He left WHEN HE saw me"). it also links certain verbs to subjects of embedded clauses ("He SAID HE was sorry").},
		:CC => %{connects clauses to following coordinating conjunctions ("SHE left BUT we stayed").},
		:CO => %{connects "openers" to subjects of clauses: "APPARENTLY / ON Tuesday , THEY went to a movie".},
		:CP => %{connects paraphrasing or quoting verbs to the wall (and, indirectly, to the paraphrased expression): "///// That is untrue, the spokesman SAID."},
		:CQ => %{connects to auxiliaries in comparative constructions involving s-v inversion: "SHE has more money THAN DOES Joe".},
		:CX => %{is used in comparative constructions where the right half of the comparative contains only an auxiliary: "She has more money THAN he DOES".},
		:D  => %{connects determiners to nouns: "THE DOG chased A CAT and SOME BIRDS".},
		:DD => %{connects definite determiners ("the", "his") to certain things like number expressions and adjectives acting as nouns: "THE POOR", "THE TWO he mentioned".},
		:DG => %{connects the word "The" with proper nouns: "the Riviera", "the Mississippi".},
		:DP => %{connects possessive determiners to gerunds: "YOUR TELLING John to leave was stupid".},
		:DT => %{connects determiners to nouns in idiomatic time expressions: "NEXT WEEK", "NEXT THURSDAY".},
		:E  => %{is used for verb-modifying adverbs which precede the verb: "He is APPARENTLY LEAVING".},
		:EA => %{connects adverbs to adjectives: "She is a VERY GOOD player".},
		:EB => %{connects adverbs to forms of "be" before an object or prepositional phrase: "He IS APPARENTLY a good programmer".},
		:EC => %{connects adverbs to comparative adjectives: "It is MUCH BIGGER"},
		:EE => %{connects adverbs to other adverbs: "He ran VERY QUICKLY".},
		:EF => %{connects the word "enough" to preceding adjectives and adverbs: "He didn't run QUICKLY ENOUGH".},
		:EI => %{connects a few adverbs to "after" and "before": "I left SOON AFTER I saw you".},
		:EL => %{connects certain words to the word "else": something / everything / anything / nothing , somewhere (etc.), and someone (etc.).},
		:EN => %{connects certain adverbs to expressions of quantity: "The class has NEARLY FIFTY students".},
		:ER => %{is used the expression "The x-er..., the y-er...". it connects the two halfs of the expression together, via the comparative words (e.g. "The FASTER it is, the MORE they will like it").},
		:EZ => %{connects certain adverbs to the word "as", like "just" and "almost": "You're JUST AS good as he is."},
		:FL => %{connects "for" to "long": "I didn't wait FOR LONG".},
		:FM => %{connects the preposition "from" to various other prepositions: "We heard a scream FROM INSIDE the house".},
		:G  => %{connects proper noun words together in series: "GEORGE HERBERT WALKER BUSH is here."},
		:GN => %{(stage 2 only) connects a proper noun to a preceding common noun which introduces it: "The ACTOR Eddie MURPHY attended the event".},
		:H  => %{connects "how" to "much" or "many": "HOW MUCH money do you have".},
		:I  => %{connects infinitive verb forms to certain words such as modal verbs and "to": "You MUST DO it", "I want TO DO it".},
		:ID => %{is a special class of link-types generated by the parser, with arbitrary four-letter names (such as "IDBT"), to connect together words of idiomatic expressions such as "at_hand" and "head_of_state".},
		:IN => %{connects the preposition "in" to certain time expressions: "We did it IN DECEMBER".},
		:J  => %{connects prepositions to their objects: "The man WITH the HAT is here".},
		:JG => %{connects certain prepositions to proper-noun objects: "The Emir OF KUWAIT is here".},
		:JQ => %{connects prepositions to question-word determiners in "prepositional questions": "IN WHICH room were you sleeping?"},
		:JT => %{connects certain conjunctions to time-expressions like "last week": "UNTIL last WEEK, I thought she liked me".},
		:K  => %{connects certain verbs with particles like "in", "out", "up" and the like: "He STOOD UP and WALKED OUT".},
		:L  => %{connects certain determiners to superlative adjectives: "He has THE BIGGEST room".},
		:LE => %{is used in comparative constructions to connect an adjective to the second half of the comparative expression beyond a complement phrase: "It is more LIKELY that Joe will go THAN that Fred will go".},
		:LI => %{connects certain verbs to the preposition "like": "I FEEL LIKE a fool."},
		:M  => %{connects nouns to various kinds of post-noun modifiers: prepositional phrases ("The MAN WITH the hat"), participle modifiers ("The WOMAN CARRYING the box"), prepositional relatives ("The MAN TO whom I was speaking"), and other kinds.},
		:MF => %{is used in the expression "Many people were injured, SOME OF THEM children".},
		:MG => %{allows certain prepositions to modify proper nouns: "The EMIR OF Kuwait is here".},
		:MV => %{connects verbs and adjectives to modifying phrases that follow, like adverbs ("The dog RAN QUICKLY"), prepositional phrases ("The dog RAN IN the yard"), subordinating conjunctions ("He LEFT WHEN he saw me"), comparatives, participle phrases with commas, and other things.},
		:MX => %{connects modifying phrases with commas to preceding nouns: "The DOG, a POODLE, was black". "JOHN, IN a black suit, looked great".},
		:N  => %{connects the word "not" to preceding auxiliaries: "He DID NOT go".},
		:ND => %{connects numbers with expressions that require numerical determiners: "I saw him THREE WEEKS ago".},
		:NF => %{is used with NJ in idiomatic number expressions involving "of": "He lives two THIRDS OF a mile from here".},
		:NI => %{is used in a few special idiomatic number phrases: "I have BETWEEN 5 AND 20 dogs".},
		:NJ => %{is used with NF in idiomatic number expressions involving "of": "He lives two thirds OF a MILE from here".},
		:NN => %{connects number words together in series: "FOUR HUNDRED THOUSAND people live here".},
		:NO => %{is used on words which have no normal linkage requirement, but need to be included in the dictionary, such as "um" and "ah".},
		:NR => %{connects fraction words with superlatives: "It is the THIRD BIGGEST city in China".},
		:NS => %{connects singular numbers (one, 1, a) to idiomatic expressions requiring number determiners: "I saw him ONE WEEK ago".},
		:NT => %{connects "not" to "to": "I told you NOT TO come".},
		:NW => %{is used in idiomatic fraction expressions: "TWO THIRDS of the students were women".},
		:O  => %{connects transitive verbs to their objects, direct or indirect: "She SAW ME", "I GAVE HIM the BOOK".},
		:OD => %{is used for verbs like "rise" and "fall" which can take expressions of distance as complements: "It FELL five FEET".},
		:OF => %{connects certain verbs and adjectives to the word "of": "She ACCUSED him OF the crime", "I'm PROUD OF you".},
		:ON => %{connectors the word "on" to dates or days of the week in time expressions: "We saw her again ON TUESDAY".},
		:OT => %{is used for verbs like "last" which can take time expressions as objects: "It LASTED five HOURS".},
		:OX => %{is an object connector, analogous to SF, used for special "filler" words like "it" and "there" when used as objects: "That MAKES IT unlikely that she will come".},
		:P  => %{connects forms of the verb "be" to various words that can be its complements: prepositions, adjectives, and passive and progressive participles: "He WAS [ ANGRY / IN the yard / CHOSEN / RUNNING ]".},
		:PF => %{is used in certain questions with "be", when the complement need of "be" is satisfied by a preceding question word: "WHERE are you?", "WHEN will it BE?"},
		:PP => %{connects forms of "have" with past participles: "He HAS GONE".},
		:Q  => %{is used in questions. It connects the wall to the auxiliary in simple yes-no questions ("///// DID you go?"); it connects the question word to the auxiliary in where-when-how questions ("WHERE DID you go").},
		:QI => %{connects certain verbs and adjectives to question-words, forming indirect questions: "He WONDERED WHAT she would say".},
		:R  => %{connects nouns to relative clauses. In subject-type relatives, it connects to the relative pronoun ("The DOG WHO chased me was black"); in object-type relatives, it connects either to the relative pronoun or to the subject of the relative clause ("The DOG THAT we chased was black", "The DOG WE chased was black").},
		:RS => %{is used in subject-type relative clauses to connect the relative pronoun to the verb: "The dog WHO CHASED me was black".},
		:RW => %{connects the right-wall to the left-wall in cases where the right-wall is not needed for punctuation purposes.},
		:S  => %{connects subject nouns to finite verbs: "The DOG CHASED the cat": "The DOG [ IS chasing / HAS chased / WILL chase ] the cat".},
		:SF => %{is a special connector used to connect "filler" subjects like "it" and "there" to finite verbs: "THERE IS a problem", "IT IS likely that he will go".},
		:SFI => %{connects "filler" subjects like "it" and "there" to verbs in cases with subject-verb inversion: "IS THERE a problem?", "IS IT likely that he will go?"},
		:SI => %{connects subject nouns to finite verbs in cases of subject-verb inversion: "IS JOHN coming?", "Who DID HE see?"},
		:SX => %{connects "I" to special first-person verbs lke "was" and "am".},
		:SXI => %{connects "I" to first-person verbs in cases of s-v inversion.},
		:TA => %{is used to connect adjectives like "late" to month names: "We did it in LATE DECEMBER".},
		:TD => %{connects day-of-the-week words to time expressions like "morning": "We'll do it MONDAY MORNING".},
		:TH => %{connects words that take "that [clause]" complements with the word "that". These include verbs ("She TOLD him THAT..."), nouns ("The IDEA THAT..."), and adjectives ("We are CERTAIN THAT").},
		:TI => %{is used for titles like "president", which can be used in certain cirumstances without a determiner: "AS PRESIDENT of the company, it is my decision".},
		:TM => %{is used to connect month names to day numbers: "It happened on JANUARY 21".},
		:TO => %{connects verbs and adjectives which take infinitival complements to the word "to": "We TRIED TO start the car", "We are EAGER TO do it".},
		:TQ => %{is the determiner connector for time expressions acting as fronted objects: "How MANY YEARS did it last".},
		:TS => %{connects certain verbs that can take subjunctive clauses as complements - "suggest", "require" - to the word that: "We SUGGESTED THAT he go".},
		:TW => %{connects days of the week to dates in time expressions: "The meeting will be on MONDAY, JANUARY 21".},
		:TY => %{is used for certain idiomatic usages of year numbers: "I saw him on January 21 , 1990 ". (In this case it connects the day number to the year number.)},
		:U  => %{is a special connector on nouns, which is disjoined with both the determiner and subject-object connectors. It is used in idiomatic expressions like "What KIND_OF DOG did you buy?"},
		:UN => %{connects the words "until" and "since" to certain time phrases like "after [clause]": "You should wait UNTIL AFTER you talk to me".},
		:V  => %{connects various verbs to idiomatic expressions that may be non-adjacent: "We TOOK him FOR_GRANTED", "We HELD her RESPONSIBLE".},
		:W  => %{connects the subjects of main clauses to the wall, in ordinary declaratives, imperatives, and most questions (except yes-no questions). It also connects coordinating conjunctions to following clauses: "We left BUT SHE stayed".},
		:WN => %{connects the word "when" to time nouns like "year": "The YEAR WHEN we lived in England was wonderful".},
		:WR => %{connects the word "where" to a few verbs like "put" in questions like "WHERE did you PUT it?".},
		:X  => %{is used with punctuation, to connect punctuation symbols either to words or to each other. For example, in this case, POODLE connects to commas on either side: "The dog , a POODLE , was black."},
		:Y  => %{is used in certain idiomatic time and place expressions, to connect quantity expressions to the head word of the expression: "He left three HOURS AGO", "She lives three MILES FROM the station".},
		:YP => %{connects plural noun forms ending in s to "'" in possessive constructions: "The STUDENTS ' rooms are large".},
		:YS => %{connects nouns to the possessive suffix "'s": "JOHN 'S dog is black".},
		:Z  => %{connects the preposition "as" to certain verbs: "AS we EXPECTED, he was late".},
	}

	# Link struct types
	Link = Struct.new( "LinkParserLink", :lword, :rword, :length, :label, :llabel, :rlabel, :desc )
	

	######
	public
	######

	### Return a human-readable representation of the Sentence object.
	def inspect
		%{#<%s:0x%x: sublinkage %d: [%d links]>} % [
			self.class.name,
			self.object_id / 2,
			self.current_sublinkage,
			self.num_links
		]
	end


	### Return the +index+th link.
	def link( index )
		Link.new( 
			self.words[ self.link_lword(index) ],
			self.words[ self.link_rword(index) ],
			self.link_length(index),
			self.link_label(index),
			self.link_llabel(index),
			self.link_rlabel(index),
			LinkTypes[ self.link_label(index).gsub(/[^A-Z]+/, '').to_sym ]
		)
	end
	

	### Return the Array of words in the sentence as tokenized by the
	### parser.
	def links
		( 0...self.link_count ).collect do |i|
			self.link( i )
		end
	end


	### Return the verb word from the linkage.
	def verb
		if link = self.links.find {|link| link.llabel =~ /^(O([DFNTX]?)|P|BI|K|LI|MV|Q)[a-z\*]*/ }
			# $deferr.puts "Picking %s: LL of %p is %s" % [ link.lword, link, link.llabel ]
			return link.lword.sub( /\.v$/, '' )
		elsif link = self.links.find {|link| link.rlabel =~ /^(SI|S|AF)[a-z\*]*/ }
			# $deferr.puts "Picking %s: RL of %p is %s" % [ link.rword, link, link.rlabel ]
			return link.rword.sub( /\.v$/, '' )
		else
			return nil
		end
	end


	### Return the subject from the linkage.
	def subject
		link = self.links.find {|link| link.llabel[0] == ?S } or return nil
		return link.lword.sub( /\.[np]$/, '' )
	end
	

	### Return the object from the linkage.
	def object
		link = self.links.find {|link| link.rlabel[0] == ?O } or return nil
		return link.rword.sub( /\.[np]$/, '' )
	end


	### Returns +true+ if the linkage indicates the sentence is phrased in the
	### imperative voice.
	def imperative?
		return self.links.find {|link| link.label == 'Wi' && link.rword =~ /\.v$/ } ?
			true : false
	end
	
	
	### Returns +true+ if the linkage has more than one sublinkage (i.e., the 
	### sentence has a conjunction).
	def has_conjunction?
		return self.num_sublinkages > 1
	end
	

end # class Sentence

