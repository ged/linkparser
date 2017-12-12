# -*- encoding: utf-8 -*-
# stub: linkparser 2.1.0.pre20171211170930 ruby lib
# stub: ext/extconf.rb

Gem::Specification.new do |s|
  s.name = "linkparser".freeze
  s.version = "2.1.0.pre20171211170930"

  s.required_rubygems_version = Gem::Requirement.new("> 1.3.1".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["Michael Granger".freeze, "Martin Chase".freeze]
  s.cert_chain = ["certs/ged.pem".freeze]
  s.date = "2017-12-12"
  s.description = "This module is a Ruby binding for\n[the Abiword version](http://www.abisource.com/projects/link-grammar/) of CMU's\n[Link Grammar](http://www.link.cs.cmu.edu/link/), a syntactic parser of English.".freeze
  s.email = ["ged@FaerieMUD.org".freeze, "stillflame@FaerieMUD.org".freeze]
  s.extensions = ["ext/extconf.rb".freeze]
  s.extra_rdoc_files = ["History.md".freeze, "Manifest.txt".freeze, "README.md".freeze, "History.md".freeze, "README.md".freeze, "ext/dictionary.c".freeze, "ext/linkage.c".freeze, "ext/linkparser.c".freeze, "ext/linkparser.h".freeze, "ext/parseoptions.c".freeze, "ext/sentence.c".freeze]
  s.files = [".simplecov".freeze, "ChangeLog".freeze, "History.md".freeze, "LICENSE".freeze, "Manifest.txt".freeze, "README.md".freeze, "Rakefile".freeze, "ext/dictionary.c".freeze, "ext/extconf.rb".freeze, "ext/linkage.c".freeze, "ext/linkparser.c".freeze, "ext/linkparser.h".freeze, "ext/parseoptions.c".freeze, "ext/sentence.c".freeze, "lib/linkparser.rb".freeze, "lib/linkparser/dictionary.rb".freeze, "lib/linkparser/linkage.rb".freeze, "lib/linkparser/mixins.rb".freeze, "lib/linkparser/parseoptions.rb".freeze, "lib/linkparser/sentence.rb".freeze, "spec/bugfixes_spec.rb".freeze, "spec/helpers.rb".freeze, "spec/linkparser/dictionary_spec.rb".freeze, "spec/linkparser/linkage_spec.rb".freeze, "spec/linkparser/mixins_spec.rb".freeze, "spec/linkparser/parseoptions_spec.rb".freeze, "spec/linkparser/sentence_spec.rb".freeze, "spec/linkparser_spec.rb".freeze]
  s.homepage = "http://deveiate.org/projects/Ruby-LinkParser".freeze
  s.licenses = ["BSD".freeze, "BSD-3-Clause".freeze]
  s.rdoc_options = ["--main".freeze, "README.md".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.0.0".freeze)
  s.rubygems_version = "2.6.14".freeze
  s.summary = "This module is a Ruby binding for [the Abiword version](http://www.abisource.com/projects/link-grammar/) of CMU's [Link Grammar](http://www.link.cs.cmu.edu/link/), a syntactic parser of English.".freeze

  if s.respond_to? :specification_version then
    s.specification_version = 4

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<loggability>.freeze, ["~> 0.11"])
      s.add_development_dependency(%q<hoe-mercurial>.freeze, ["~> 1.4"])
      s.add_development_dependency(%q<hoe-deveiate>.freeze, ["~> 0.9"])
      s.add_development_dependency(%q<hoe-highline>.freeze, ["~> 0.2"])
      s.add_development_dependency(%q<rake-compiler>.freeze, ["~> 0"])
      s.add_development_dependency(%q<rdoc-generator-fivefish>.freeze, ["~> 0"])
      s.add_development_dependency(%q<rdoc>.freeze, ["~> 4.0"])
      s.add_development_dependency(%q<hoe>.freeze, ["~> 3.16"])
    else
      s.add_dependency(%q<loggability>.freeze, ["~> 0.11"])
      s.add_dependency(%q<hoe-mercurial>.freeze, ["~> 1.4"])
      s.add_dependency(%q<hoe-deveiate>.freeze, ["~> 0.9"])
      s.add_dependency(%q<hoe-highline>.freeze, ["~> 0.2"])
      s.add_dependency(%q<rake-compiler>.freeze, ["~> 0"])
      s.add_dependency(%q<rdoc-generator-fivefish>.freeze, ["~> 0"])
      s.add_dependency(%q<rdoc>.freeze, ["~> 4.0"])
      s.add_dependency(%q<hoe>.freeze, ["~> 3.16"])
    end
  else
    s.add_dependency(%q<loggability>.freeze, ["~> 0.11"])
    s.add_dependency(%q<hoe-mercurial>.freeze, ["~> 1.4"])
    s.add_dependency(%q<hoe-deveiate>.freeze, ["~> 0.9"])
    s.add_dependency(%q<hoe-highline>.freeze, ["~> 0.2"])
    s.add_dependency(%q<rake-compiler>.freeze, ["~> 0"])
    s.add_dependency(%q<rdoc-generator-fivefish>.freeze, ["~> 0"])
    s.add_dependency(%q<rdoc>.freeze, ["~> 4.0"])
    s.add_dependency(%q<hoe>.freeze, ["~> 3.16"])
  end
end
