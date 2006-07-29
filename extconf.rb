#!/usr/bin/ruby
#
#	extconf.rb - Extension config script for the Ruby LinkParser library
#
#	Author: Michael Granger (with lots of code borrowed from the bdb Ruby
#				extension's extconf.rb)
#
#	Copyright (c) 2006, The FaerieMUD Consortium.
#
#   This work is licensed under the Creative Commons Attribution License. To
#   view a copy of this license, visit
#   http://creativecommons.org/licenses/by/1.0 or send a letter to Creative
#   Commons, 559 Nathan Abbott Way, Stanford, California 94305, USA.

require 'mkmf'
require 'rbconfig'


### Generate a rule
def rule( target, clean = nil )
   wr = "#{target}:
\t@for subdir in $(SUBDIRS); do \\
\t\t$(MAKE) -C $${subdir} #{target}; \\
\tdone;
"
   if clean != nil
     # wr << "\t@-rm tmp/* tests/tmp/* 2> /dev/null\n"
	  wr << "\t@-rm -f mkmf.log ext/mkmf.log 2> /dev/null\n"
	  wr << "\t@-rm -f ext/depend 2> /dev/null\n"
      wr << "\t@rm Makefile\n" if clean
   end
   wr
end


### Invoke the extconf files in the ext directory
rubybin = Config::CONFIG['RUBY_INSTALL_NAME']

$deferr.puts "#{$0}: Entering directory `ext'"
Dir.chdir( "ext" ) do
	system( "#{rubybin} extconf.rb " + ARGV.join(" ") )
end
$deferr.puts("#{$0}: Leaving directory `ext'")


### Write the main makefile
File.open( "Makefile", "w" ) do |ofh|
	ofh.print <<-EOF

SUBDIRS := ext

#{rule('all')}
#{rule('clean', false)}
#{rule('distclean', true)}
#{rule('realclean', true)}
#{rule('install')}
#{rule('depend')}
#{rule('site-install')}
#{rule('unknown')}

.PHONY: docs test debugtest

docs: docs/makedocs.rb
	docs/makedocs.rb -v

test: all
	testrb -a tests/*.tests.rb

debugtest: clean all
	ruby -wd -e <<EOF \
	require 'test/unit' \
	(r = Test::Unit::AutoRunner.new(true)).process_args(ARGV) or \
	  abort r.options.banner + " tests..." \
	exit r.run \
	EOF
end

