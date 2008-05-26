#!rake
#
# Ruby LinkParser library rakefile
#
# Copyright (c) 2008, The FaerieMUD Consortium
#
# Authors:
# * Michael Granger <mgranger@laika.com>
#

BEGIN {
	require 'pathname'
	basedir = Pathname.new( __FILE__ ).dirname
	libdir = basedir + 'lib'
	docsdir = basedir + 'docs'

	$LOAD_PATH.unshift( libdir.to_s ) unless $LOAD_PATH.include?( libdir.to_s )
	$LOAD_PATH.unshift( docsdir.to_s ) unless $LOAD_PATH.include?( docsdir.to_s )
}


require 'rbconfig'
require 'rubygems'
require 'rake'
require 'rake/rdoctask'
require 'rake/packagetask'
require 'rake/gempackagetask'
require 'pathname'
require 'mkrf'


include Config
$dryrun = false

# Pathname constants
BASEDIR       = Pathname.new( __FILE__ ).dirname.expand_path
LIBDIR        = BASEDIR + 'lib'
EXTDIR        = BASEDIR + 'ext'
DOCSDIR       = BASEDIR + 'docs'
MISCDIR       = BASEDIR + 'misc'
RDOCDIR       = DOCSDIR + 'rdoc'
PKGDIR        = BASEDIR + 'pkg'
ARTIFACTS_DIR = Pathname.new( ENV['CC_BUILD_ARTIFACTS'] || '' )

TEXT_FILES    = %w( Rakefile README INSTALL ).
	collect {|filename| BASEDIR + filename }

SPECDIR       = BASEDIR + 'spec'
SPEC_FILES    = Pathname.glob( SPECDIR + '**/*_spec.rb' ).
	delete_if {|item| item =~ /\.svn/ }
SPEC_EXCLUDES = 'spec,monkeypatches,/Library/Ruby'

LIB_FILES     = Pathname.glob( LIBDIR + '**/*.rb' ).
	delete_if {|item| item =~ /\.svn/ }
EXT_FILES     = Pathname.glob( EXTDIR + '**/*.{c,h}' ).
	delete_if {|item| item =~ /\.svn/ }
EXT_RAKEFILE  = EXTDIR + 'Rakefile'
EXT_SO        = EXTDIR + "linkparser_ext.#{CONFIG['DLEXT']}"

RELEASE_FILES = TEXT_FILES + LIB_FILES + EXT_FILES + SPEC_FILES


### Rake helper functions
require MISCDIR + 'rake/helpers'

### Package constants
PKG_NAME      = 'linkparser'
PKG_VERSION   = find_pattern_in_file( /VERSION = '(\d+\.\d+\.\d+)'/, LIBDIR + 'linkparser.rb' ).first
PKG_FILE_NAME = "#{PKG_NAME}-#{PKG_VERSION}"

RELEASE_NAME  = "REL #{PKG_VERSION}"

# Load task plugins
RAKE_TASKDIR = MISCDIR + 'rake'
Pathname.glob( RAKE_TASKDIR + '*.rb' ).each do |tasklib|
	next if tasklib =~ %r{/helpers.rb$}
	require tasklib
end

if Rake.application.options.trace
	$trace = true
	log "$trace is enabled"
end

if Rake.application.options.dryrun
	$dryrun = true
	log "$dryrun is enabled"
	Rake.application.options.dryrun = false
end


#####################################################################
###	T A S K S
#####################################################################

### Default task
task :default  => [:build, :spec, :package]
task :all => :default


### Task: clean
desc "Clean pkg, coverage, and rdoc; remove .bak files"
task :clean => [ :clobber_rdoc, :clobber_package, :clobber_coverage ] do
	files = FileList['**/*.bak']
	files.clear_exclude
	File.rm( files ) unless files.empty?
	FileUtils.rm_rf( 'artifacts' )

	if EXT_RAKEFILE.exist?
		Dir.chdir( EXTDIR ) do
			sh 'rake', 'clobber'
		end
		EXT_RAKEFILE.delete
	end
end


### Task: docs -- Convenience task for rebuilding dynamic docs
task :docs => [ :coverage, :rdoc ]


### Task: build -- Build the C extension half

file EXT_RAKEFILE.to_s => FileList[ 'Rakefile', EXTDIR + '*.c' ] do
	require 'misc/monkeypatches' # Fix Mkrf's output
	
	log "Configuring linkparser C extension"
	Dir.chdir( EXTDIR ) do
		Mkrf::Generator.new( 'linkparser_ext', FileList['*.c'] ) do |gen|
			trace "Setting CFLAGS"
			gen.cflags << ' -Wall'
			gen.cflags << ' -DDEBUG'
			trace "Checking for dictionary_create()"
			gen.include_library( "link-grammar", "dictionary_create" ) or
				fail( "Could not find link-grammar library." )
			trace "Checking for link-grammar/link-includes.h"
			gen.include_header( "link-grammar/link-includes.h" ) or
				fail( "Could not find link-includes.h" )
			trace "Checking for link-grammar/utilities.h"
			gen.include_header( "link-grammar/utilities.h" )
			trace "Checking for patched link-grammar library via linkage_get_current_sublinkage()"
			gen.has_function?( "linkage_get_current_sublinkage" ) or
				fail "Link grammar library is unpatched."
		end
	end
end

task :build => EXT_RAKEFILE.to_s do
	Dir.chdir( EXTDIR ) do
		sh 'rake'
	end
end


### Task: rdoc
Rake::RDocTask.new do |rdoc|
	rdoc.rdoc_dir = 'docs/api'
	rdoc.title    = "LinkParser - a natural language framework for Ruby"

	rdoc.options += [
		'-w', '4',
		'-SHN',
		'-i', 'docs',
		'-f', 'darkfish',
		'-m', 'README',
		'-W', 'http://deveiate.org/projects/linkparser/trunk/'
	  ]
	
	rdoc.rdoc_files.include TEXT_FILES.collect {|f| f.relative_path_from(BASEDIR).to_s }
	rdoc.rdoc_files.include LIB_FILES.collect {|f| f.relative_path_from(BASEDIR).to_s }
end


### Task: gem
gemspec = Gem::Specification.new do |gem|
	pkg_build = get_svn_rev( BASEDIR ) || 0
	
	gem.name    	= PKG_NAME
	gem.version 	= "%s.%s" % [ PKG_VERSION, pkg_build ]

	gem.summary     = "a Ruby binding for the link-grammar library"
	gem.description = <<-EOD
	A Ruby binding for the link-grammar library, a syntactic English parser.
	EOD

	gem.authors  	= "Michael Granger, Martin Chase"
	gem.email       = "ged@FaerieMUD.org, stillflame@FaerieMUD.org"
	gem.homepage 	= "http://deveiate.org/projects/Ruby-LinkParser/"

	gem.has_rdoc 	= true

	gem.files      	= RELEASE_FILES.
		collect {|f| f.relative_path_from(BASEDIR).to_s }
	gem.test_files 	= SPEC_FILES.
		collect {|f| f.relative_path_from(BASEDIR).to_s }
end
Rake::GemPackageTask.new( gemspec ) do |task|
	task.gem_spec = gemspec
	task.need_tar = false
	task.need_tar_gz = true
	task.need_tar_bz2 = true
	task.need_zip = true
end


### Task: install
desc "Install LinkParser as a conventional library"
task :install do
	log "Installing LinkParser as a convention library"
	sitelib = Pathname.new( CONFIG['sitelibdir'] )
	Dir.chdir( LIBDIR ) do
		LIB_FILES.each do |libfile|
			relpath = libfile.relative_path_from( LIBDIR )
			target = sitelib + relpath
			FileUtils.mkpath target.dirname,
				:mode => 0755, :verbose => true, :noop => $dryrun unless target.dirname.directory?
			FileUtils.install relpath, target,
				:mode => 0644, :verbose => true, :noop => $dryrun
		end
	end
	Dir.chdir( EXTDIR ) do
		sh 'rake', 'install'
	end
end

### Task: install_gem
desc "Install LinkParser as a gem"
task :install_gem => [:package] do
	installer = Gem::Installer.new( %{pkg/#{PKG_FILE_NAME}.gem} )
	installer.install
end

desc "Uninstall LinkParser if it's been installed as a conventional library"
task :uninstall do
	log "Uninstalling conventionally-installed LinkParser library files"
	sitelib = Pathname.new( CONFIG['sitelibdir'] )
	dir = sitelib + 'linkparser'
	FileUtils.rm_rf( dir, :verbose => true, :noop => $dryrun )
	lib = sitelib + 'linkparser.rb'
	FileUtils.rm( lib, :verbose => true, :noop => $dryrun )
end

### Task: uninstall_gem
task :uninstall_gem => [:clean] do
	uninstaller = Gem::Uninstaller.new( PKG_FILE_NAME )
	uninstaller.uninstall
end



### Cruisecontrol task
desc "Cruisecontrol build"
task :cruise => [:clean, :coverage, :package] do |task|
	raise "Artifacts dir not set." if ARTIFACTS_DIR.to_s.empty?
	artifact_dir = ARTIFACTS_DIR.cleanpath
	artifact_dir.mkpath
	
	$stderr.puts "Copying coverage stats..."
	FileUtils.cp_r( 'coverage', artifact_dir )
	
	$stderr.puts "Copying packages..."
	FileUtils.cp_r( FileList['pkg/*'].to_a, artifact_dir )
end


### RSpec tasks
begin
	gem 'rspec', '>= 1.1.1'
	require 'spec/rake/spectask'

	COMMON_SPEC_OPTS = ['-c', '-f', 's']

	### Task: spec
	Spec::Rake::SpecTask.new( :spec ) do |task|
		task.spec_files = SPEC_FILES
		task.libs += [LIBDIR]
		task.spec_opts = COMMON_SPEC_OPTS
	end
	task :test => [:spec]


	namespace :spec do
		desc "Run rspec every time there's a change to one of the files"
        task :autotest do
            require 'autotest/rspec'
            autotester = Autotest::Rspec.new

			autotester.exceptions = %r{\.svn|\.skel}
            autotester.run
        end

	
		desc "Generate HTML output for a spec run"
		Spec::Rake::SpecTask.new( :html ) do |task|
			task.spec_files = SPEC_FILES
			task.spec_opts = ['-f','h', '-D']
		end

		desc "Generate plain-text output for a CruiseControl.rb build"
		Spec::Rake::SpecTask.new( :text ) do |task|
			task.spec_files = SPEC_FILES
			task.spec_opts = ['-f','p']
		end
	end
rescue LoadError => err
	task :no_rspec do
		$stderr.puts "Testing tasks not defined: RSpec rake tasklib not available: %s" %
			[ err.message ]
	end
	
	task :spec => :no_rspec
	namespace :spec do
		task :autotest => :no_rspec
		task :html => :no_rspec
		task :text => :no_rspec
	end
end


### RCov (via RSpec) tasks
begin
	gem 'rcov'
	gem 'rspec', '>= 1.1.1'

	RCOV_OPTS = ['--exclude', SPEC_EXCLUDES, '--xrefs', '--save']

	### Task: coverage (via RCov)
	### Task: spec
	desc "Build test coverage reports"
	Spec::Rake::SpecTask.new( :coverage ) do |task|
		task.spec_files = SPEC_FILES
		task.libs += LIB_FILES
		task.spec_opts = ['-f', 'p', '-b']
		task.rcov_opts = RCOV_OPTS
		task.rcov = true
	end
	
	task :rcov => [:coverage] do; end
	
	### Other coverage tasks
	namespace :coverage do
		desc "Generate a detailed text coverage report"
		Spec::Rake::SpecTask.new( :text ) do |task|
			task.spec_files = SPEC_FILES
			task.libs += FileList['plugins/**/lib']
			task.rcov_opts = RCOV_OPTS + ['--text-report']
			task.rcov = true
		end

		desc "Show differences in coverage from last run"
		Spec::Rake::SpecTask.new( :diff ) do |task|
			task.spec_files = SPEC_FILES
			task.libs += FileList['plugins/**/lib']
			task.rcov_opts = ['--text-coverage-diff']
			task.rcov = true
		end

		### Task: verify coverage
		desc "Build coverage statistics"
		VerifyTask.new( :verify => :rcov ) do |task|
			task.threshold = 85.0
		end
		
		desc "Run RCov in 'spec-only' mode to check coverage from specs"
		Spec::Rake::SpecTask.new( :speconly ) do |task|
			task.spec_files = SPEC_FILES
			task.libs += FileList['plugins/**/lib']
			task.rcov_opts = ['--exclude', SPEC_EXCLUDES, '--text-report', '--save']
			task.rcov = true
		end
	end

rescue LoadError => err
	task :no_rcov do
		$stderr.puts "Coverage tasks not defined: RSpec+RCov tasklib not available: %s" %
			[ err.message ]
	end

	task :coverage => :no_rcov
	task :clobber_coverage
	task :rcov => :no_rcov
	namespace :coverage do
		task :text => :no_rcov
		task :diff => :no_rcov
	end
	task :verify => :no_rcov
end



### Coding style checks and fixes
namespace :style do
	
	BLANK_LINE = /^\s*$/
	GOOD_INDENT = /^(\t\s*)?\S/

	# A list of the files that have legitimate leading whitespace, etc.
	PROBLEM_FILES = [ SPECDIR + 'config_spec.rb' ]
	
	desc "Check source files for inconsistent indent and fix them"
	task :fix_indent do
		files = LIB_FILES + SPEC_FILES

		badfiles = Hash.new {|h,k| h[k] = [] }
		
		trace "Checking files for indentation"
		files.each do |file|
			if PROBLEM_FILES.include?( file )
				trace "  skipping problem file #{file}..."
				next
			end
			
			trace "  #{file}"
			linecount = 0
			file.each_line do |line|
				linecount += 1
				
				# Skip blank lines
				next if line =~ BLANK_LINE
				
				# If there's a line with incorrect indent, note it and skip to the 
				# next file
				if line !~ GOOD_INDENT
					trace "    Bad line %d: %p" % [ linecount, line ]
					badfiles[file] << [ linecount, line ]
				end
			end
		end

		if badfiles.empty?
			log "No indentation problems found."
		else
			log "Found incorrect indent in #{badfiles.length} files:\n  "
			badfiles.each do |file, badlines|
				log "  #{file}:\n" +
					"    " + badlines.collect {|badline| "%5d: %p" % badline }.join( "\n    " )
			end
		end
	end

end


