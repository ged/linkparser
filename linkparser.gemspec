# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{linkparser}
  s.version = "1.1.1.20110514145649"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = [%q{Michael Granger}, %q{Martin Chase}]
  s.date = %q{2011-05-14}
  s.description = %q{This module is a Ruby binding for [the Abiword version][abiword] of CMU's 
[Link Grammar][link-grammar], a syntactic parser of English.}
  s.email = [%q{ged@FaerieMUD.org}, %q{stillflame@FaerieMUD.org}]
  s.extensions = [%q{ext/extconf.rb}]
  s.files = [%q{ChangeLog}, %q{History.md}, %q{LICENSE}, %q{README.md}, %q{Rakefile}, %q{ext/dictionary.c}, %q{ext/extconf.rb}, %q{ext/linkage.c}, %q{ext/linkparser.c}, %q{ext/linkparser.h}, %q{ext/parseoptions.c}, %q{ext/sentence.c}, %q{lib/linkparser.rb}, %q{lib/linkparser/linkage.rb}, %q{lib/linkparser/mixins.rb}, %q{lib/linkparser/sentence.rb}, %q{spec/bugfixes_spec.rb}, %q{spec/linkparser/dictionary_spec.rb}, %q{spec/linkparser/linkage_spec.rb}, %q{spec/linkparser/mixins_spec.rb}, %q{spec/linkparser/parseoptions_spec.rb}, %q{spec/linkparser/sentence_spec.rb}, %q{spec/linkparser_spec.rb}]
  s.homepage = %q{http://deveiate.org/projects/Ruby-LinkParser}
  s.licenses = [%q{BSD}]
  s.rdoc_options = [%q{--main}, %q{README.md}]
  s.require_paths = [%q{lib}]
  s.required_ruby_version = Gem::Requirement.new(">= 1.8.7")
  s.rubyforge_project = %q{linkparser}
  s.rubygems_version = %q{1.8.2}
  s.summary = %q{This module is a Ruby binding for [the Abiword version][abiword] of CMU's  [Link Grammar][link-grammar], a syntactic parser of English.}

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<rake-compiler>, ["~> 0.7"])
      s.add_development_dependency(%q<rspec>, ["~> 2.4"])
      s.add_development_dependency(%q<hoe>, [">= 2.9.4"])
    else
      s.add_dependency(%q<rake-compiler>, ["~> 0.7"])
      s.add_dependency(%q<rspec>, ["~> 2.4"])
      s.add_dependency(%q<hoe>, [">= 2.9.4"])
    end
  else
    s.add_dependency(%q<rake-compiler>, ["~> 0.7"])
    s.add_dependency(%q<rspec>, ["~> 2.4"])
    s.add_dependency(%q<hoe>, [">= 2.9.4"])
  end
end
