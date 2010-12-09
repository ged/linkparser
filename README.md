# Ruby-LinkParser

This module is a Ruby binding for the link-grammar library, a syntactic parser
of English. 

## Authors

* Michael Granger <ged@FaerieMUD.org>
* Martin Chase <stillflame@FaerieMUD.org>


## Requirements

* Ruby 1.8.7 or 1.9.2
* link-grammar (version 4.7.0 or later) from the AbiWord project 
  (http://www.abisource.com/projects/link-grammar/) 


## Installation

These instructions assume a UNIX or UNIX-like environment. I've built it on
MacOS X, FreeBSD, and Ubuntu Linux. I'm not sure how to get this to build
under Windows, as I don't have a Windows license with which to test it.
Suggestions welcomed.

First, you'll need to install the Abiword link-grammar library that comes with
this source (or download it yourself if you wish):

    $ tar -xvzf link-grammar-4.7.0.tar.gz
    $ cd link-grammar-4.7.0
    $ ./configure; make; sudo make install
    $ cd ..

Now build, test, and install the Ruby library:

    $ rake
    $ sudo rake install

If you've installed the link-grammar library someplace that isn't in your
regular include path, you might have to tell the build system where to look:

    $ rake -- --with-link-grammar-dir=/usr/local

Under MacOS X, unless you've taken extra steps to compile the link-grammar
library as a universal binary, you'll probably also have to limit it to
your machine's architecture:

    $ ARCHFLAGS="-arch i386" rake -- --with-link-grammar-dir=/usr/local

That's it!


## Example Usage

    require 'linkparser'
    
    dict = LinkParser::Dictionary.new( :screen_width => 100 )
    sent = dict.parse( "People use Ruby for all kinds of nifty things." )
    # => #<LinkParser::Sentence:0xcf8eb "LEFT-WALL people use Ruby for all kinds
    #      of nifty things . RIGHT-WALL"/2 linkages/0 nulls>
    
    sent.subject        # => "people"
    sent.verb           # => "use"
    sent.object         # => "Ruby"
    
    puts sent.constituent_tree_string
    # =>
    # (S (NP People)
    #    (VP use
    #        (NP Ruby)
    #        (PP for
    #            (NP (NP all kinds)
    #                (PP of
    #                    (NP nifty things)))))
    #    .)
    
    puts sent.diagram
    # =>
    #     +-------------------------------Xp------------------------------+
    #     |                +----MVp---+----Jp----+     +------Jp-----+    |
    #     +----Wd---+--Sp--+--Os-+    |    +-Dmc-+--Mp-+    +----A---+    |
    #     |         |      |     |    |    |     |     |    |        |    |
    # LEFT-WALL people.p use.v Ruby for.p all kinds.n of nifty.a things.n . 


## Legal

For licensing information, see the LICENSE file.

For copyright and licensing information for link-grammar itself, see the
LICENSE file in that distribution.

 $Id$


