# LinkParser rakefile
# 
# Based on the rakefiles of a long list of projects,
# including plist, Gruff, Rake, Linen, ThingFish, and
# others.
#
# Copyright (c) 2007, 2008 The FaerieMUD Consortium
#
# Mistakes:
#  * Michael Granger <ged@FaerieMUD.org>
#  * Ben Bleything <ben@bleything.net>
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname
	libdir = basedir + "lib"
	extdir = basedir + "ext"
	docsdir = basedir + "docs"
	
	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
	$LOAD_PATH.unshift( docsdir.to_s ) unless $LOAD_PATH.include?( docsdir.to_s )
}

require 'fileutils'
require 'pp'

require 'rake'
require 'rake/packagetask'
require 'rake/gempackagetask'
require 'rake/rdoctask'

require 'spec/rake/spectask'
require 'spec/rake/verify_rcov'

TEXT_FILES = %w( Rakefile README MANIFEST )
SPEC_FILES = Dir.glob( 'spec/*_spec.rb' )
LIB_FILES  = Dir.glob( 'lib/**/*.rb' )
EXT_FILES  = Dir.glob( 'ext/*.c' ) + Dir.glob( 'ext/*.h' )
BIN_FILES  = Dir.glob( 'bin/*' )

RELEASE_FILES = TEXT_FILES +
				SPEC_FILES +
				BIN_FILES  +
				LIB_FILES

task :default  => 'spec:normal'

###########################
### DOCUMENTATION TASKS ###
###########################

# Task: rdoc
Rake::RDocTask.new { |rdoc|
	rdoc.rdoc_dir = 'docs/api'
	rdoc.title    = "Ruby LinkParser library"

	rdoc.options += [
		'-w', '4',
		'-SHN',
		'-i', 'docs',
		'-f', 'darkfish',
		'-m', 'README',
		'-W', 'http://deveiate.org/projects/Ruby-LinkParser/browser/trunk/'
	  ]
	
	rdoc.rdoc_files.include 'README'
	rdoc.rdoc_files.include LIB_FILES
	rdoc.rdoc_files.include EXT_FILES
}


###########################
###    TESTING TASKS    ###
###########################

### Task: spec
task :spec => 'spec:normal'
namespace :spec do
	desc "Run specs outputting to STDOUT"
	Spec::Rake::SpecTask.new( :normal ) do |task|
		task.ruby_opts << '-rubygems'
		task.libs << 'lib' << 'ext'
		task.spec_files = SPEC_FILES
		task.spec_opts = ['-f s', '-r linkparser', '-c']
	end

    desc "Generate plaintext output"
    Spec::Rake::SpecTask.new( :text ) do |task|
		task.ruby_opts << '-rubygems'
		task.libs << 'lib' << 'ext'
        task.spec_files = SPEC_FILES
        task.spec_opts = ['-f s', '-r linkparser']
    end

    desc "Generate HTML output for a spec run"
    Spec::Rake::SpecTask.new( :html ) do |task|
		task.ruby_opts << '-rubygems'
		task.libs << 'lib' << 'ext'
        task.spec_files = SPEC_FILES
        task.spec_opts = ['-f h', '-r linkparser', '-c']
    end

	task :autotest do
		require 'rspec_autotest'
		
		ra = RspecAutotest.new
		ra.spec_directory = 'spec'
		
		ra.run
	end
end


### Task: rcov
desc "Check coverage"
Spec::Rake::SpecTask.new( :coverage ) do |task|
	task.libs << 'lib'
	task.spec_files = SPEC_FILES
	task.rcov = true
end

### Task: verify_rcov
desc "Build coverage statistics"
RCov::VerifyTask.new( :verify_coverage => :coverage ) do |task|
	task.threshold = 85.0
end

