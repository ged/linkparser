# -*- ruby -*-
#encoding: utf-8

require 'yaml'

require 'linkparser' unless defined?( LinkParser )
require 'linkparser/mixins'

# This is the API's representation of a parse. A LinkParser::Sentence may have one or more
# of LinkParser::Linkages, each of which represents one possible structure of the sentence.
# It can be thought of as a Sentence together with a collection of links. If the parse
# has a conjunction, then the Linkage is made up of at least two "sublinkages". A
# Linkage can be pretty printed in either ASCII or Postscript format, and individual
# links can be extracted.
class LinkParser::Linkage
	extend Loggability,
	       LinkParser::DeprecationUtilities

	# Use LinkParser's logger
	log_to :linkparser


	# Descriptions of the linkage types, keyed by linkage symbol
	# Loaded from the data section of this file, which is generated with
	# the experiments/build_linktype_hash.rb script
	LINK_TYPES = YAML.load( IO.read(__FILE__).split(/^__END__/, 2).last )

	# Link struct (:lword, :rword, :length, :label, :llabel, :rlabel, :desc)
	Link = Struct.new( "LinkParserLink", :lword, :rword, :length, :label, :llabel, :rlabel, :desc )



	######
	public
	######

	### Return a human-readable representation of the Sentence object.
	def inspect
		return %{#<%s:0x%x: [%d links]>} % [
			self.class.name,
			self.object_id / 2,
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
			LINK_TYPES[ self.link_label(index).gsub(/[^A-Z]+/, '') ]
		)
	end


	### Return the Array of words in the sentence as tokenized by the
	### parser.
	def links
		return ( 0...self.link_count ).collect do |i|
			self.link( i )
		end
	end


	### Return an Array of parsed (well, just split on whitespace for now) disjunct strings
	### for the linkage.
	def disjuncts
		return self.disjunct_strings.collect do |dstr|
			if dstr.nil?
				nil
			else
				dstr.split
			end
		end
	end


	### Return the verb word from the linkage.
	def verb( keep_subscript: false )
		word = if verblink = self.links.find {|link| link.llabel =~ /^(O([DFNTX]?)|P|BI|K|LI|MV|Q)[a-z\*]*/ }
				verblink.lword
			elsif verblink = self.links.find {|link| link.rlabel =~ /^(SI|S|AF)[a-z\*]*/ }
				verblink.rword
			else
				nil
			end

		return with_subscript( word, keep_subscript )
	end


	### Return the subject from the linkage.
	def subject( keep_subscript: false )
		subjlink = self.links.find {|link| link.llabel[0] == ?S } or return nil
		return with_subscript( subjlink.lword, keep_subscript )
	end


	### Return the object from the linkage.
	def object( keep_subscript: false )
		objlink = self.links.find {|link| link.rlabel[0] == ?O } or return nil
		return with_subscript( objlink.rword, keep_subscript )
	end


	### Return an Array of all the nouns in the linkage.
	def nouns
		nouns = []
		self.links.each do |link|
			nouns << $1 if link.lword =~ /^(.*)\.n(?:-\w)?$/
			nouns << $1 if link.rword =~ /^(.*)\.n(?:-\w)?$/
		end

		return nouns.uniq
	end


	### Returns +true+ if the linkage indicates the sentence is phrased in the
	### imperative voice.
	def imperative?
		return self.links.find {|link| link.label == 'Wi' && link.rword =~ /\.v$/ } ?
			true : false
	end


	#######
	private
	#######


	### Return the specified +word+ with the part-of-speech subscript removed.
	def with_subscript( word, keep_subscript )
		return unless word
		return word if keep_subscript
		return word.sub( /\.[\p{alpha}\-]+$/, '' )
	end

end # class Sentence


__END__
---
A: 'connects pre-noun ("attributive") adjectives to following nouns: "The BIG DOG
  chased me", "The BIG BLACK UGLY DOG chased me".'
AA: is used in the construction "How [adj] a [noun] was it?". It connects the adjective
  to the following "a".
AF: 'connects adjectives to verbs in cases where the adjective is fronted, such as
  questions and indirect questions: "How BIG IS it?"'
AJ: 'connects adjectives to conjunctions to form a modifier phrase: "The BLACK AND
  WHITE cat sleeps."'
AL: 'connects a few determiners like "all" or "both" to following determiners: "ALL
  THE people are here".'
AM: 'connects "as" to "much" or "many": "I don''t go out AS MUCH now".'
AN: 'connects noun-modifiers to following nouns: "The TAX PROPOSAL was rejected".'
AZ: 'connects the word "as" back to certain verbs that can take "[obj] as [adj]" as
  a complement: "He VIEWED him AS stupid".'
B: serves various functions involving relative clauses and questions. It connects
  transitive verbs back to their objects in relative clauses, questions, and indirect
  questions ("The DOG we CHASED", "WHO did you SEE?"); it also connects the main noun
  to the finite verb in subject-type relative clauses ("The DOG who CHASED me was
  black").
BI: 'connects forms of the verb "be" to certain idiomatic expressions: for example,
  cases like "He IS PRESIDENT of the company".'
BT: 'is used with time expressions acting as fronted objects: "How many YEARS did
  it LAST?".'
BW: 'connects "what" to various verbs like "think", which are not really transitive
  but can connect back to "what" in questions: "WHAT do you THINK?"'
C: links conjunctions to subjects of subordinate clauses ("He left WHEN HE saw me").
  it also links certain verbs to subjects of embedded clauses ("He SAID HE was sorry").
CC: connects clauses to following coordinating conjunctions ("SHE left BUT we stayed").
CO: 'connects "openers" to subjects of clauses: "APPARENTLY / ON Tuesday , THEY went
  to a movie".'
CP: 'connects paraphrasing or quoting verbs to the wall (and, indirectly, to the paraphrased
  expression): "///// That is untrue, the spokesman SAID."'
CQ: 'connects to auxiliaries in comparative constructions involving s-v inversion:
  "SHE has more money THAN DOES Joe".'
CV: connects the verbs of subordinate clauses to the subordinating word.
CX: 'is used in comparative constructions where the right half of the comparative
  contains only an auxiliary: "She has more money THAN he DOES".'
D: 'connects determiners to nouns: "THE DOG chased A CAT and SOME BIRDS".'
DD: 'connects definite determiners ("the", "his") to certain things like number expressions
  and adjectives acting as nouns: "THE POOR", "THE TWO he mentioned".'
DG: 'connects the word "The" with proper nouns: "the Riviera", "the Mississippi".'
DP: 'connects possessive determiners to gerunds: "YOUR TELLING John to leave was stupid".'
DT: 'connects determiners to nouns in idiomatic time expressions: "NEXT WEEK", "NEXT
  THURSDAY".'
E: 'is used for verb-modifying adverbs which precede the verb: "He is APPARENTLY LEAVING".'
EA: 'connects adverbs to adjectives: "She is a VERY GOOD player".'
EB: 'connects adverbs to forms of "be" before an object or prepositional phrase: "He
  IS APPARENTLY a good programmer".'
EC: 'connects adverbs to comparative adjectives: "It is MUCH BIGGER"'
EE: 'connects adverbs to other adverbs: "He ran VERY QUICKLY".'
EF: 'connects the word "enough" to preceding adjectives and adverbs: "He didn''t run
  QUICKLY ENOUGH".'
EI: 'connects a few adverbs to "after" and "before": "I left SOON AFTER I saw you".'
EL: 'connects certain words to the word "else": something / everything / anything
  / nothing , somewhere (etc.), and someone (etc.).'
EN: 'connects certain adverbs to expressions of quantity: "The class has NEARLY FIFTY
  students".'
EQ: 'connects parts of an equation together: "Phosphorylation was observed (P = 0.06)".'
ER: is used the expression "The x-er..., the y-er...". it connects the two halfs of
  the expression together, via the comparative words (e.g. "The FASTER it is, the
  MORE they will like it").
EZ: 'connects certain adverbs to the word "as", like "just" and "almost": "You''re
  JUST AS good as he is."'
FL: 'connects "for" to "long": "I didn''t wait FOR LONG".'
FM: 'connects the preposition "from" to various other prepositions: "We heard a scream
  FROM INSIDE the house".'
G: 'connects proper noun words together in series: "GEORGE HERBERT WALKER BUSH is
  here."'
GN: '(stage 2 only) connects a proper noun to a preceding common noun which introduces
  it: "The ACTOR Eddie MURPHY attended the event".'
H: 'connects "how" to "much" or "many": "HOW MUCH money do you have".'
HA: 'connects "how" to "a": "HOW big A dog was it?"'
I: 'connects infinitive verb forms to certain words such as modal verbs and "to":
  "You MUST DO it", "I want TO DO it".'
ID: |-
  is a special class of link-types generated by the parser, with arbitrary
  four-letter names (such as "IDBT"), to connect together words of
  idiomatic expressions such as "at_hand" and "head_of_state".
IN: 'connects the preposition "in" to certain time expressions: "We did it IN DECEMBER".'
IV: connects the infinitive verb to the clause that starts the infinitive.
J: 'connects prepositions to their objects: "The man WITH the HAT is here".'
JG: 'connects certain prepositions to proper-noun objects: "The Emir OF KUWAIT is
  here".'
JQ: 'connects prepositions to question-word determiners in "prepositional questions":
  "IN WHICH room were you sleeping?"'
JT: 'connects certain conjunctions to time-expressions like "last week": "UNTIL last
  WEEK, I thought she liked me".'
K: 'connects certain verbs with particles like "in", "out", "up" and the like: "He
  STOOD UP and WALKED OUT".'
L: 'connects certain determiners to superlative adjectives: "He has THE BIGGEST room".'
LE: 'is used in comparative constructions to connect an adjective to the second half
  of the comparative expression beyond a complement phrase: "It is more LIKELY that
  Joe will go THAN that Fred will go".'
LI: 'connects certain verbs to the preposition "like": "I FEEL LIKE a fool."'
M: 'connects nouns to various kinds of post-noun modifiers: prepositional phrases
  ("The MAN WITH the hat"), participle modifiers ("The WOMAN CARRYING the box"), prepositional
  relatives ("The MAN TO whom I was speaking"), and other kinds.'
MF: is used in the expression "Many people were injured, SOME OF THEM children".
MG: 'allows certain prepositions to modify proper nouns: "The EMIR OF Kuwait is here".'
MJ: 'connects prepositions and other post-nominal modifiers to conjunctions to form
  a prepositional or modifier phrase: "It is hidden somewhere IN OR NEAR the house."'
MV: connects verbs and adjectives to modifying phrases that follow, like adverbs ("The
  dog RAN QUICKLY"), prepositional phrases ("The dog RAN IN the yard"), subordinating
  conjunctions ("He LEFT WHEN he saw me"), comparatives, participle phrases with commas,
  and other things.
MX: 'connects modifying phrases with commas to preceding nouns: "The DOG, a POODLE,
  was black". "JOHN, IN a black suit, looked great".'
N: 'connects the word "not" to preceding auxiliaries: "He DID NOT go".'
NA: 'connects numbers used in spelled-out dates: "The war started in NINETEEN FOURTEEN".'
ND: 'connects numbers with expressions that require numerical determiners: "I saw
  him THREE WEEKS ago".'
NF: 'is used with NJ in idiomatic number expressions involving "of": "He lives two
  THIRDS OF a mile from here".'
NI: 'is used in a few special idiomatic number phrases: "I have BETWEEN 5 AND 20 dogs".'
NJ: 'is used with NF in idiomatic number expressions involving "of": "He lives two
  thirds OF a MILE from here".'
NM: 'connects certain idiomatic numerical modifiers: "He is on FLIGHT 714", "That
  will cost $300".'
NN: 'connects number words together in series: "FOUR HUNDRED THOUSAND people live
  here".'
'NO': is used on words which have no normal linkage requirement, but need to be included
  in the dictionary, such as "um" and "ah".
NR: 'connects fraction words with superlatives: "It is the THIRD BIGGEST city in China".'
NS: 'connects singular numbers (one, 1, a) to idiomatic expressions requiring number
  determiners: "I saw him ONE WEEK ago".'
NT: 'connects "not" to "to": "I told you NOT TO come".'
NW: 'is used in idiomatic fraction expressions: "TWO THIRDS of the students were women".'
O: 'connects transitive verbs to their objects, direct or indirect: "She SAW ME",
  "I GAVE HIM the BOOK".'
OD: 'is used for verbs like "rise" and "fall" which can take expressions of distance
  as complements: "It FELL five FEET".'
OF: 'connects certain verbs and adjectives to the word "of": "She ACCUSED him OF the
  crime", "I''m PROUD OF you".'
'ON': 'connectors the word "on" to dates or days of the week in time expressions:
  "We saw her again ON TUESDAY".'
OT: 'is used for verbs like "last" which can take time expressions as objects: "It
  LASTED five HOURS".'
OX: 'is an object connector, analogous to SF, used for special "filler" words like
  "it" and "there" when used as objects: "That MAKES IT unlikely that she will come".'
P: 'connects forms of the verb "be" to various words that can be its complements:
  prepositions, adjectives, and passive and progressive participles: "He WAS [ ANGRY
  / IN the yard / CHOSEN / RUNNING ]".'
PF: 'is used in certain questions with "be", when the complement need of "be" is satisfied
  by a preceding question word: "WHERE are you?", "WHEN will it BE?"'
PH: 'is used to enforce phonetic agreement in the usage of "a" and "an" with words
  that begin with consonants and vowels. So: "I ate AN apple." "I ate A green apple."'
PP: 'connects forms of "have" with past participles: "He HAS GONE".'
Q: is used in questions. It connects the wall to the auxiliary in simple yes-no questions
  ("///// DID you go?"); it connects the question word to the auxiliary in where-when-how
  questions ("WHERE DID you go").
QI: 'connects certain verbs and adjectives to question-words, forming indirect questions:
  "He WONDERED WHAT she would say".'
QJ: 'connects question words to conjunctions to form a question phrase: "WHEN AND
  WHERE is the party?"'
R: connects nouns to relative clauses. In subject-type relatives, it connects to the
  relative pronoun ("The DOG WHO chased me was black"); in object-type relatives,
  it connects either to the relative pronoun or to the subject of the relative clause
  ("The DOG THAT we chased was black", "The DOG WE chased was black").
RJ: 'provides support for conjoining adverbs and other miscellaneous phrases: "She
  handled it QUICKLY AND GRACEFULLY"'
RS: 'is used in subject-type relative clauses to connect the relative pronoun to the
  verb: "The dog WHO CHASED me was black".'
RW: connects the right-wall to the left-wall in cases where the right-wall is not
  needed for punctuation purposes.
S: 'connects subject nouns to finite verbs: "The DOG CHASED the cat": "The DOG [ IS
  chasing / HAS chased / WILL chase ] the cat".'
SF: 'is a special connector used to connect "filler" subjects like "it" and "there"
  to finite verbs: "THERE IS a problem", "IT IS likely that he will go".'
SFI: 'connects "filler" subjects like "it" and "there" to verbs in cases with subject-verb
  inversion: "IS THERE a problem?", "IS IT likely that he will go?"'
SI: 'connects subject nouns to finite verbs in cases of subject-verb inversion: "IS
  JOHN coming?", "Who DID HE see?"'
SJ: 'connects nouns to conjunctions to form a noun phrase: "I have the BALL AND CHAIN
  with me tonight."'
SX: connects "I" to special first-person verbs lke "was" and "am".
SXI: connects "I" to first-person verbs in cases of s-v inversion.
TA: 'is used to connect adjectives like "late" to month names: "We did it in LATE
  DECEMBER".'
TD: 'connects day-of-the-week words to time expressions like "morning": "We''ll do
  it MONDAY MORNING".'
TH: connects words that take "that [clause]" complements with the word "that". These
  include verbs ("She TOLD him THAT..."), nouns ("The IDEA THAT..."), and adjectives
  ("We are CERTAIN THAT").
TI: 'is used for titles like "president", which can be used in certain cirumstances
  without a determiner: "AS PRESIDENT of the company, it is my decision".'
TM: 'is used to connect month names to day numbers: "It happened on JANUARY 21".'
TO: 'connects verbs and adjectives which take infinitival complements to the word
  "to": "We TRIED TO start the car", "We are EAGER TO do it".'
TQ: 'is the determiner connector for time expressions acting as fronted objects: "How
  MANY YEARS did it last".'
TR: 'connects determiners to comparatives: "The better the computer, the faster the
  program."'
TS: 'connects certain verbs that can take subjunctive clauses as complements - "suggest",
  "require" - to the word that: "We SUGGESTED THAT he go".'
TW: 'connects days of the week to dates in time expressions: "The meeting will be
  on MONDAY, JANUARY 21".'
TY: 'is used for certain idiomatic usages of year numbers: "I saw him on January 21
  , 1990 ". (In this case it connects the day number to the year number.)'
TZ: 'is used for certain idiomatic usages of time zones: "The meeting starts at 1
  p.m. EDT."'
U: is a special connector on nouns, which is disjoined with both the determiner and
  subject-object connectors. It is used in idiomatic expressions like "What KIND_OF
  DOG did you buy?"
UN: 'connects the words "until" and "since" to certain time phrases like "after [clause]":
  "You should wait UNTIL AFTER you talk to me".'
V: 'connects various verbs to idiomatic expressions that may be non-adjacent: "We
  TOOK him FOR_GRANTED", "We HELD her RESPONSIBLE".'
VJ: 'connects verbs to conjunctions: "He RAN AND JUMPED".'
W: 'connects the subjects of main clauses to the wall, in ordinary declaratives, imperatives,
  and most questions (except yes-no questions). It also connects coordinating conjunctions
  to following clauses: "We left BUT SHE stayed".'
WN: 'connects the word "when" to time nouns like "year": "The YEAR WHEN we lived in
  England was wonderful".'
WR: connects the word "where" to a few verbs like "put" in questions like "WHERE did
  you PUT it?".
WV: connects the verbs of main clauses to the wall.
X: 'is used with punctuation, to connect punctuation symbols either to words or to
  each other. For example, in this case, POODLE connects to commas on either side:
  "The dog , a POODLE , was black."'
XJ: 'provides support for various idiomatic coordinating expressions, such as "...
  NOT ONLY x, BUT y": "You should NOT ONLY ask for your money back, BUT demand it."'
Y: 'is used in certain idiomatic time and place expressions, to connect quantity expressions
  to the head word of the expression: "He left three HOURS AGO", "She lives three
  MILES FROM the station".'
YP: 'connects plural noun forms ending in s to "''" in possessive constructions: "The
  STUDENTS '' rooms are large".'
YS: 'connects nouns to the possessive suffix "''s": "JOHN ''S dog is black".'
Z: 'connects the preposition "as" to certain verbs: "AS we EXPECTED, he was late".'
