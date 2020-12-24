# -*- encoding: utf-8 -*-
# stub: linkparser 2.3.0.pre.20201224135544 ruby lib
# stub: ext/linkparser_ext/extconf.rb

Gem::Specification.new do |s|
  s.name = "linkparser".freeze
  s.version = "2.3.0.pre.20201224135544"

  s.required_rubygems_version = Gem::Requirement.new("> 1.3.1".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "bug_tracker_uri" => "https://todo.sr.ht/~ged/linkparser/browse", "changelog_uri" => "http://deveiate.org/code/linkparser/History_md.html", "documentation_uri" => "http://deveiate.org/code/linkparser", "homepage_uri" => "https://hg.sr.ht/~ged/linkparser", "source_uri" => "https://hg.sr.ht/~ged/linkparser/browse" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["Michael Granger".freeze]
  s.date = "2020-12-24"
  s.description = "This module is a Ruby binding for the Abiword version of CMU&#39;s Link Grammar, a syntactic parser of English.".freeze
  s.email = ["ged@faeriemud.org".freeze]
  s.extensions = ["ext/linkparser_ext/extconf.rb".freeze]
  s.files = ["History.md".freeze, "README.md".freeze, "ext/linkparser_ext/dictionary.c".freeze, "ext/linkparser_ext/extconf.rb".freeze, "ext/linkparser_ext/linkage.c".freeze, "ext/linkparser_ext/linkparser.c".freeze, "ext/linkparser_ext/linkparser.h".freeze, "ext/linkparser_ext/parseoptions.c".freeze, "ext/linkparser_ext/sentence.c".freeze, "lib/linkparser.rb".freeze, "lib/linkparser/dictionary.rb".freeze, "lib/linkparser/linkage.rb".freeze, "lib/linkparser/mixins.rb".freeze, "lib/linkparser/parseoptions.rb".freeze, "lib/linkparser/sentence.rb".freeze, "spec/bugfixes_spec.rb".freeze, "spec/helpers.rb".freeze, "spec/linkparser/dictionary_spec.rb".freeze, "spec/linkparser/linkage_spec.rb".freeze, "spec/linkparser/mixins_spec.rb".freeze, "spec/linkparser/parseoptions_spec.rb".freeze, "spec/linkparser/sentence_spec.rb".freeze, "spec/linkparser_spec.rb".freeze]
  s.homepage = "https://hg.sr.ht/~ged/linkparser".freeze
  s.licenses = ["BSD-3-Clause".freeze]
  s.rubygems_version = "3.1.4".freeze
  s.summary = "This module is a Ruby binding for the Abiword version of CMU&#39;s Link Grammar, a syntactic parser of English.".freeze

  if s.respond_to? :specification_version then
    s.specification_version = 4
  end

  if s.respond_to? :add_runtime_dependency then
    s.add_runtime_dependency(%q<loggability>.freeze, ["~> 0.17"])
    s.add_runtime_dependency(%q<rake-compiler>.freeze, ["~> 1.1"])
    s.add_runtime_dependency(%q<rdoc>.freeze, ["~> 6.2"])
    s.add_development_dependency(%q<rake-deveiate>.freeze, ["~> 0.15"])
    s.add_development_dependency(%q<rdoc-generator-fivefish>.freeze, ["~> 0.4"])
  else
    s.add_dependency(%q<loggability>.freeze, ["~> 0.17"])
    s.add_dependency(%q<rake-compiler>.freeze, ["~> 1.1"])
    s.add_dependency(%q<rdoc>.freeze, ["~> 6.2"])
    s.add_dependency(%q<rake-deveiate>.freeze, ["~> 0.15"])
    s.add_dependency(%q<rdoc-generator-fivefish>.freeze, ["~> 0.4"])
  end
end
