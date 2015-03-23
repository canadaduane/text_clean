# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'text_clean'

Gem::Specification.new do |spec|
  spec.name          = "text_clean"
  spec.version       = TextClean::VERSION
  spec.authors       = ["Duane Johnson"]
  spec.email         = ["duane.johnson@gmail.com"]
  spec.description   = %q{Cleans text by removing punctuation, lowercasing. Very fast.}
  spec.summary       = %q{Text cleaner}
  spec.homepage      = "https://github.com/wordtreefoundation"
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib", "ext"]

  spec.extensions    = %w[ext/extconf.rb]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake", "~> 10.3"
  spec.add_development_dependency "byebug", "~> 3.4"
end
