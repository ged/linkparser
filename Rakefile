#!rake
#
# LinkParser rakefile
#
# Based on various other Rakefiles, especially one by Ben Bleything
#
# Copyright (c) 2008 The FaerieMUD Consortium
#
# Authors:
#  * Michael Granger <ged@FaerieMUD.org>
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname

	libdir = basedir + "lib"
	extdir = basedir + "ext"

	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( extdir.to_s ) unless $LOAD_PATH.include?( extdir.to_s )
}


require 'rbconfig'
require 'rubygems'
require 'rake'
require 'rake/rdoctask'
require 'rake/testtask'
require 'rake/packagetask'
require 'rake/clean'

$dryrun = false

### Config constants
BASEDIR       = Pathname.new( __FILE__ ).dirname.relative_path_from( Pathname.getwd )
LIBDIR        = BASEDIR + 'lib'
EXTDIR        = BASEDIR + 'ext'
DOCSDIR       = BASEDIR + 'docs'
PKGDIR        = BASEDIR + 'pkg'

PKG_NAME      = 'linkparser'
PKG_SUMMARY   = 'a Ruby binding for the link-grammar library'
VERSION_FILE  = LIBDIR + 'linkparser.rb'
PKG_VERSION   = VERSION_FILE.read[ /VERSION = '(\d+\.\d+\.\d+)'/, 1 ]
PKG_FILE_NAME = "#{PKG_NAME.downcase}-#{PKG_VERSION}"
GEM_FILE_NAME = "#{PKG_FILE_NAME}.gem"

ARTIFACTS_DIR = Pathname.new( ENV['CC_BUILD_ARTIFACTS'] || 'artifacts' )

TEXT_FILES    = %w( Rakefile ChangeLog README LICENSE ).collect {|filename| BASEDIR + filename }
LIB_FILES     = Pathname.glob( LIBDIR + '**/*.rb' ).delete_if {|item| item =~ /\.svn/ }
EXT_FILES     = Pathname.glob( EXTDIR + '**/*.{c,h,rb}' ).delete_if {|item| item =~ /\.svn/ }

SPECDIR       = BASEDIR + 'spec'
SPEC_FILES    = Pathname.glob( SPECDIR + '**/*_spec.rb' ).delete_if {|item| item =~ /\.svn/ }

TESTDIR       = BASEDIR + 'tests'
TEST_FILES    = Pathname.glob( TESTDIR + '**/*.tests.rb' ).delete_if {|item| item =~ /\.svn/ }

RAKE_TASKDIR  = BASEDIR + 'rake'
RAKE_TASKLIBS = Pathname.glob( RAKE_TASKDIR + '*.rb' )

LOCAL_RAKEFILE = BASEDIR + 'Rakefile.local'

EXTRA_PKGFILES = []
EXTRA_PKGFILES.concat Pathname.glob( BASEDIR + 'link-grammar*.{patch,tar.gz}' ).delete_if {|item| item =~ /\.svn/ } 
EXTRA_PKGFILES.concat Pathname.glob( BASEDIR + 'misc/monkeypatches.rb' ).delete_if {|item| item =~ /\.svn/ } 
EXTRA_PKGFILES.concat Pathname.glob( BASEDIR + 'loadpath.rb' ).delete_if {|item| item =~ /\.svn/ } 

RELEASE_FILES = TEXT_FILES + 
	SPEC_FILES + 
	TEST_FILES + 
	LIB_FILES + 
	EXT_FILES + 
	RAKE_TASKLIBS +
	EXTRA_PKGFILES

RELEASE_FILES << LOCAL_RAKEFILE if LOCAL_RAKEFILE.exist?

COVERAGE_MINIMUM = ENV['COVERAGE_MINIMUM'] ? Float( ENV['COVERAGE_MINIMUM'] ) : 85.0
RCOV_EXCLUDES = 'spec,tests,/Library/Ruby,/var/lib,/usr/local/lib'
RCOV_OPTS = [
	'--exclude', RCOV_EXCLUDES,
	'--xrefs',
	'--save',
	'--callsites',
	#'--aggregate', 'coverage.data' # <- doesn't work as of 0.8.1.2.0
  ]


# Subversion constants -- directory names for releases and tags
SVN_TRUNK_DIR    = 'trunk'
SVN_RELEASES_DIR = 'releases'
SVN_BRANCHES_DIR = 'branches'
SVN_TAGS_DIR     = 'tags'

SVN_DOTDIR       = BASEDIR + '.svn'
SVN_ENTRIES      = SVN_DOTDIR + 'entries'


### Load some task libraries that need to be loaded early
require RAKE_TASKDIR + 'helpers.rb'
require RAKE_TASKDIR + 'svn.rb'
require RAKE_TASKDIR + 'verifytask.rb'

# Define some constants that depend on the 'svn' tasklib
PKG_BUILD = get_svn_rev( BASEDIR ) || 0
SNAPSHOT_PKG_NAME = "#{PKG_FILE_NAME}.#{PKG_BUILD}"
SNAPSHOT_GEM_NAME = "#{SNAPSHOT_PKG_NAME}.gem"

# Documentation constants
RDOC_OPTIONS = [
	'-w', '4',
	'-SHN',
	'-i', '.',
	'-m', 'README',
	'-W', 'http://deveiate.org/projects/Ruby-LinkParser//browser/trunk/'
  ]

# Release constants
SMTP_HOST = 'mail.faeriemud.org'
SMTP_PORT = 465 # SMTP + SSL

# Project constants
PROJECT_HOST = 'deveiate.org'
PROJECT_PUBDIR = "/usr/local/www/public/code"
PROJECT_DOCDIR = "#{PROJECT_PUBDIR}/#{PKG_NAME}"
PROJECT_SCPURL = "#{PROJECT_HOST}:#{PROJECT_DOCDIR}"

# Rubyforge stuff
RUBYFORGE_GROUP = 'deveiate'
RUBYFORGE_PROJECT = 'linkparser'

# Gem dependencies: gemname => version
DEPENDENCIES = {
}

# Non-gem requirements: packagename => version
REQUIREMENTS = {
	'link-grammar' => '= 4.3.5',
}

# RubyGem specification
GEMSPEC   = Gem::Specification.new do |gem|
	gem.name              = PKG_NAME.downcase
	gem.version           = PKG_VERSION

	gem.summary           = PKG_SUMMARY
	gem.description       = <<-EOD
	A Ruby binding for the link-grammar library, a syntactic parser
	of English. See http://www.link.cs.cmu.edu/link/ for more
	information about the Link Grammar, and
	http://www.abisource.org/projects/link-grammar/ for information
	about the link-grammar library.
	EOD

	gem.authors           = 'Michael Granger'
	gem.email             = 'ged@FaerieMUD.org'
	gem.homepage          = 'http://deveiate.org/projects/Ruby-LinkParser/'
	gem.rubyforge_project = RUBYFORGE_PROJECT

	gem.has_rdoc          = true
	gem.rdoc_options      = RDOC_OPTIONS

	gem.files             = RELEASE_FILES.
		collect {|f| f.relative_path_from(BASEDIR).to_s }
	gem.test_files        = SPEC_FILES.
		collect {|f| f.relative_path_from(BASEDIR).to_s }
		
	DEPENDENCIES.each do |name, version|
		version = '>= 0' if version.length.zero?
		gem.add_dependency( name, version )
	end
	
	REQUIREMENTS.each do |name, version|
		gem.requirements << [ name, version ].compact.join(' ')
	end
end

$trace = Rake.application.options.trace ? true : false
$dryrun = Rake.application.options.dryrun ? true : false


# Load any remaining task libraries
RAKE_TASKLIBS.each do |tasklib|
	next if tasklib =~ %r{/(helpers|svn|verifytask)\.rb$}
	begin
		require tasklib
	rescue ScriptError => err
		fail "Task library '%s' failed to load: %s: %s" %
			[ tasklib, err.class.name, err.message ]
		trace "Backtrace: \n  " + err.backtrace.join( "\n  " )
	rescue => err
		log "Task library '%s' failed to load: %s: %s. Some tasks may not be available." %
			[ tasklib, err.class.name, err.message ]
		trace "Backtrace: \n  " + err.backtrace.join( "\n  " )
	end
end

# Load any project-specific rules defined in 'Rakefile.local' if it exists
import LOCAL_RAKEFILE if LOCAL_RAKEFILE.exist?


#####################################################################
###	T A S K S 	
#####################################################################

### Default task
task :default  => [:clean, :local, :spec, :rdoc, :package]

### Task the local Rakefile can append to -- no-op by default
task :local


### Task: clean
CLEAN.include 'coverage'
CLOBBER.include 'artifacts', 'coverage.info', PKGDIR

# Target to hinge on ChangeLog updates
file SVN_ENTRIES

### Task: changelog
file 'ChangeLog' => SVN_ENTRIES.to_s do |task|
	log "Updating #{task.name}"

	changelog = make_svn_changelog()
	File.open( task.name, 'w' ) do |fh|
		fh.print( changelog )
	end
end


### Task: cruise (Cruisecontrol task)
desc "Cruisecontrol build"
task :cruise => [:clean, :spec, :package] do |task|
	raise "Artifacts dir not set." if ARTIFACTS_DIR.to_s.empty?
	artifact_dir = ARTIFACTS_DIR.cleanpath
	artifact_dir.mkpath
	
	$stderr.puts "Copying coverage stats..."
	FileUtils.cp_r( 'coverage', artifact_dir )
	
	$stderr.puts "Copying packages..."
	FileUtils.cp_r( FileList['pkg/*'].to_a, artifact_dir )
end


desc "Update the build system to the latest version"
task :update_build do
	log "Updating the build system"
	sh 'svn', 'up', RAKE_TASKDIR
	log "Updating the Rakefile"
	sh 'rake', '-f', RAKE_TASKDIR + 'Metarakefile'
end

