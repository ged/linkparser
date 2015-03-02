# -*- encoding: utf-8 -*-
# stub: linkparser 2.1.0.pre20150302085132 ruby lib
# stub: ext/extconf.rb

Gem::Specification.new do |s|
  s.name = "linkparser"
  s.version = "2.1.0.pre20150302085132"

  s.required_rubygems_version = Gem::Requirement.new("> 1.3.1") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Michael Granger", "Martin Chase"]
  s.date = "2015-03-02"
  s.description = "This module is a Ruby binding for \n[the Abiword version](http://www.abisource.com/projects/link-grammar/) of CMU's\n[Link Grammar](http://www.link.cs.cmu.edu/link/), a syntactic parser of English."
  s.email = ["ged@FaerieMUD.org", "stillflame@FaerieMUD.org"]
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["History.md", "Manifest.txt", "README.md", "History.md", "README.md", "ext/dictionary.c", "ext/linkage.c", "ext/linkparser.c", "ext/linkparser.h", "ext/parseoptions.c", "ext/sentence.c"]
  s.files = [".simplecov", "ChangeLog", "History.md", "LICENSE", "Manifest.txt", "README.md", "Rakefile", "ext/dictionary.c", "ext/extconf.rb", "ext/linkage.c", "ext/linkparser.c", "ext/linkparser.h", "ext/parseoptions.c", "ext/sentence.c", "lib/linkparser.rb", "lib/linkparser/dictionary.rb", "lib/linkparser/linkage.rb", "lib/linkparser/mixins.rb", "lib/linkparser/parseoptions.rb", "lib/linkparser/sentence.rb", "spec/bugfixes_spec.rb", "spec/helpers.rb", "spec/linkparser/dictionary_spec.rb", "spec/linkparser/linkage_spec.rb", "spec/linkparser/mixins_spec.rb", "spec/linkparser/parseoptions_spec.rb", "spec/linkparser/sentence_spec.rb", "spec/linkparser_spec.rb"]
  s.homepage = "http://deveiate.org/projects/Ruby-LinkParser"
  s.licenses = ["BSD"]
  s.rdoc_options = ["--main", "README.md"]
  s.required_ruby_version = Gem::Requirement.new(">= 2.0.0")
  s.rubygems_version = "2.4.5"
  s.summary = "This module is a Ruby binding for  [the Abiword version](http://www.abisource.com/projects/link-grammar/) of CMU's [Link Grammar](http://www.link.cs.cmu.edu/link/), a syntactic parser of English."

  if s.respond_to? :specification_version then
    s.specification_version = 4

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<loggability>, ["~> 0.11"])
      s.add_development_dependency(%q<hoe-mercurial>, ["~> 1.4"])
      s.add_development_dependency(%q<hoe-deveiate>, ["~> 0.6"])
      s.add_development_dependency(%q<hoe-highline>, ["~> 0.2"])
      s.add_development_dependency(%q<rdoc>, ["~> 4.0"])
      s.add_development_dependency(%q<rake-compiler>, ["~> 0"])
      s.add_development_dependency(%q<rdoc-generator-fivefish>, ["~> 0"])
      s.add_development_dependency(%q<hoe>, ["~> 3.13"])
    else
      s.add_dependency(%q<loggability>, ["~> 0.11"])
      s.add_dependency(%q<hoe-mercurial>, ["~> 1.4"])
      s.add_dependency(%q<hoe-deveiate>, ["~> 0.6"])
      s.add_dependency(%q<hoe-highline>, ["~> 0.2"])
      s.add_dependency(%q<rdoc>, ["~> 4.0"])
      s.add_dependency(%q<rake-compiler>, ["~> 0"])
      s.add_dependency(%q<rdoc-generator-fivefish>, ["~> 0"])
      s.add_dependency(%q<hoe>, ["~> 3.13"])
    end
  else
    s.add_dependency(%q<loggability>, ["~> 0.11"])
    s.add_dependency(%q<hoe-mercurial>, ["~> 1.4"])
    s.add_dependency(%q<hoe-deveiate>, ["~> 0.6"])
    s.add_dependency(%q<hoe-highline>, ["~> 0.2"])
    s.add_dependency(%q<rdoc>, ["~> 4.0"])
    s.add_dependency(%q<rake-compiler>, ["~> 0"])
    s.add_dependency(%q<rdoc-generator-fivefish>, ["~> 0"])
    s.add_dependency(%q<hoe>, ["~> 3.13"])
  end
end
