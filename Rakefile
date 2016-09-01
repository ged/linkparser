#!/usr/bin/env rake

require 'rbconfig'
require 'pathname'

begin
	require 'hoe'
rescue LoadError => err
	abort "This Rakefile requires 'hoe' (gem install hoe)."
end

begin
	require 'rake/extensiontask'
rescue LoadError => err
	abort "This Rakefile requires 'rake-compiler' (gem install rake-compiler)"
end

# Build constants
BASEDIR = Pathname( __FILE__ ).dirname.relative_path_from( Pathname.pwd )
SPECDIR = BASEDIR + 'spec'
EXTDIR  = BASEDIR + 'ext'
LIBDIR  = BASEDIR + 'lib'

DLEXT   = RbConfig::CONFIG['DLEXT']

EXTCONF = EXTDIR + 'extconf.rb'
EXT     = LIBDIR + "linkparser_ext.#{DLEXT}"

GEMSPEC = 'linkparser.gemspec'

# Hoe plugins
Hoe.plugin :mercurial
Hoe.plugin :publish
Hoe.plugin :signing

Hoe.plugins.delete :rubyforge

# Main gem configuration
hoespec = Hoe.spec 'linkparser' do
	self.license 'BSD'
	self.readme_file = 'README.md'
	self.history_file = 'History.md'
	self.extra_rdoc_files = FileList[ '*.md', 'ext/*.[ch]' ]

	self.developer 'Michael Granger', 'ged@FaerieMUD.org'
	self.developer 'Martin Chase', 'stillflame@FaerieMUD.org'

	self.dependency 'loggability', '~> 0.11'
	self.dependency 'rake-compiler', '~> 0', :development
	self.dependency 'hoe-deveiate', '~> 0.5', :development
	self.dependency 'rdoc-generator-fivefish', '~> 0', :development

	self.spec_extras[:extensions] = [ EXTCONF.to_s ]

	self.require_ruby_version( '>=2.0.0' )
	self.hg_sign_tags = true if self.respond_to?( :hg_sign_tags= )
	self.check_history_on_release = true if self.respond_to?( :check_history_on_release= )
	self.rdoc_locations << "deveiate:/usr/local/www/public/code/#{remote_rdoc_dir}"
end

ENV['VERSION'] ||= hoespec.spec.version.to_s

# Running the tests depends on compilation
# Need to (re)compile before running specs
task :spec => :compile

desc "Turn on warnings and debugging in the build."
task :maint do
	ENV['MAINTAINER_MODE'] = 'yes'
end

ENV['RUBY_CC_VERSION'] = '2.0.0:2.1:2.2'

# Rake-compiler task
Rake::ExtensionTask.new do |ext|
	ext.gem_spec       = hoespec.spec
	ext.name           = 'linkparser_ext'
	ext.ext_dir        = 'ext'
	ext.lib_dir        = 'lib'
	ext.source_pattern = "*.{c,h}"
	ext.cross_compile  = true
	ext.cross_platform = %w[i386-mswin32 i386-mingw32]
end


# Ensure the specs pass before checking in
task 'hg:precheckin' => [:check_history, :check_manifest, :gemspec, :spec]

desc "Build a coverage report"
task :coverage do
	ENV["COVERAGE"] = 'yes'
	Rake::Task[:spec].invoke
end


# Use the fivefish formatter for docs generated from development checkout
if File.directory?( '.hg' )
	require 'rdoc/task'

	Rake::Task[ 'docs' ].clear
	RDoc::Task.new( 'docs' ) do |rdoc|
	rdoc.main = "README.rdoc"
	rdoc.rdoc_files.include( "*.rdoc", "ChangeLog", "lib/**/*.rb", "ext/**/*.c" )
	rdoc.generator = :fivefish
	rdoc.title = "Ruby LinkParser"
	rdoc.rdoc_dir = 'doc'
	end
end

task :gemspec => GEMSPEC
file GEMSPEC => __FILE__
task GEMSPEC do |task|
	spec = $hoespec.spec
	spec.files.delete( '.gemtest' )
	spec.version = "#{spec.version.bump}.0.pre#{Time.now.strftime("%Y%m%d%H%M%S")}"
	spec.signing_key = nil
	spec.cert_chain = [ 'certs/ged.pem' ]
	File.open( task.name, 'w' ) do |fh|
		fh.write( spec.to_ruby )
	end
end

CLOBBER.include( GEMSPEC.to_s )
task :default => :gemspec
