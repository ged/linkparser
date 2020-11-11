# linkparser

home
: https://hg.sr.ht/~ged/linkparser

code
: https://hg.sr.ht/~ged/linkparser/browse

docs
: http://deveiate.org/code/linkparser

github
: http://github.com/ged/linkparser



## Description

This module is a Ruby binding for
[the Abiword version](http://www.abisource.com/projects/link-grammar/) of CMU's
[Link Grammar](http://www.link.cs.cmu.edu/link/), a syntactic parser of English.


### Example Usage

    dict = LinkParser::Dictionary.new
    => #<LinkParser::Dictionary:0x007fc56b480d18 @options={}>

    sent = dict.parse( "People use Ruby for all kinds of nifty things." )
    => #<LinkParser::Sentence:0x1ff15b42eeda
          "LEFT-WALL people.p use.v Ruby.f for.p all.a kinds.n of nifty.a things.n . RIGHT-WALL"/
          15 linkages/0 nulls>

    sent.subject
    => "people"

    sent.verb
    => "use"

    sent.object
    => "Ruby"

    puts sent.diagram( max_width: 200 )
    
        +---------------------------------Xp--------------------------------+
        |                             +---------------Jp---------------+    |
        |                             |            +--------Dmc--------+    |
        +------>WV------>+-----MVp----+            |     +------Jd-----+    |
        +----Wd---+--Sp--+--Os--+     |     +-Dmcn-+-OFd-+    +----A---+    +--RW--+
        |         |      |      |     |     |      |     |    |        |    |      |
    LEFT-WALL people.p use.v Ruby.f for.p all.a kinds.n of nifty.a things.n . RIGHT-WALL


## Installation

First, download and install the latest version of the link-grammar
library from
[Abiword's site](http://www.abisource.com/projects/link-grammar/#download).

Then install the gem:

    gem install linkparser

You may need to specify the path to the link-grammar library if you
installed it somewhere that your linker doesn't look by default:

    gem install linkparser -- --with-link-grammar=/usr/local


## Contributing

You can check out the current development source
[with Mercurial](https://hg.sr.ht/~ged/linkparser),
or if you prefer Git, via
[its Github mirror](http://github.com/ged/linkparser).

After checking out the source, run:

    $ gen install -Ng
    $ rake setup

This will install any missing dependencies and do any neceesary developer setup.


## Author(s)

- Michael Granger <ged@faeriemud.org>


## License

This gem uses the link-grammar library, which is licensed under the
LGPL v2.1 license:

https://www.gnu.org/licenses/lgpl-2.1.html

The gem itself is licensed under the BSD license:

Copyright (c) 2006-2020, The FaerieMUD Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the author/s, nor the names of the project's
  contributors may be used to endorse or promote products derived from this
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

