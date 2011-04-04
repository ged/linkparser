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
BASEDIR = Pathname( __FILE__ ).dirname
SPECDIR = BASEDIR + 'spec'
EXTDIR  = BASEDIR + 'ext'
LIBDIR  = BASEDIR + 'lib'

DLEXT   = Config::CONFIG['DLEXT']

EXTCONF = EXTDIR + 'extconf.rb'
EXT     = LIBDIR + "linkparser_ext.#{DLEXT}"

# Hoe plugins
Hoe.plugin :mercurial
Hoe.plugin :signing

Hoe.plugins.delete :rubyforge

# Main gem configuration
hoespec = Hoe.spec 'linkparser' do
	self.readme_file = 'README.md'
	self.history_file = 'History.md'

	self.developer 'Michael Granger', 'ged@FaerieMUD.org'
	self.developer 'Martin Chase', 'stillflame@FaerieMUD.org'

	self.extra_deps.push *{
		'rake-compiler' => '~> 0.7',
	}
	self.extra_dev_deps.push *{
		'rspec'         => '~> 2.4',
	}

	self.spec_extras[:licenses] = ["BSD"]
	self.spec_extras[:extensions] = [ EXTCONF.to_s ]

	self.require_ruby_version( '>=1.8.7' )

	self.hg_sign_tags = true if self.respond_to?( :hg_sign_tags= )
	self.rdoc_locations << "deveiate:/usr/local/www/public/code/#{remote_rdoc_dir}"
end

ENV['VERSION'] ||= hoespec.spec.version.to_s

# Running the tests depends on compilation
# Need to (re)compile before running specs
task :spec => :compile

# gem-testers support
task :test do
	# rake-compiler always wants to copy the compiled extension into lib/, but
	# we don't want testers to have to re-compile, especially since that
	# often fails because they can't (and shouldn't have to) write to tmp/ in
	# the installed gem dir. So we clear the task rake-compiler set up
	# to break the dependency between :spec and :compile when running under
	# rubygems-test, and then run :spec.
	Rake::Task[ EXT.to_s ].clear
	Rake::Task[ :spec ].execute
end

desc "Turn on warnings and debugging in the build."
task :maint do
	ENV['MAINTAINER_MODE'] = 'yes'
end

ENV['RUBY_CC_VERSION'] = '1.8.7:1.9.2'

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

