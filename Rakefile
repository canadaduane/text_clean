
require 'rspec'
require 'rspec/core/rake_task'

# `rake test`
desc 'Run all examples'
RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = 'spec/**/*_spec.rb'
end

# `rake compile`
require "rake/extensiontask"

Rake::ExtensionTask.new "text_clean" do |ext|
  ext.lib_dir = "lib/text_clean"
  ext.source_pattern = "*.{c,cc}"
end