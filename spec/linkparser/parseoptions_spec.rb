# -*- ruby -*-
#encoding: utf-8

require_relative '../helpers'

require 'rspec'
require 'linkparser'


describe LinkParser::ParseOptions do

	let( :opts ) { described_class.new }


	it "starts out with documented defaults" do
		expect( opts.verbosity ).to eq( 1 )             # Docs say this is 0
		expect( opts.linkage_limit ).to eq( 100 )       # Docs say this is 10000
		expect( opts.disjunct_cost ).to eq( 2 )
		expect( opts.min_null_count ).to eq( 0 )
		expect( opts.max_null_count ).to eq( 0 )
		expect( opts.islands_ok? ).to eq( false )
		expect( opts.short_length ).to eq( 16 )
		expect( opts.max_memory ).to eq( -1 )
		expect( opts.max_parse_time ).to eq( -1 )
		expect( opts.all_short_connectors? ).to eq( false )
		expect( opts.cost_model_type ).to eq( :vdal )
	end


	it "knows whether the timer constraints were exceeded or not" do
		expect( opts.timer_expired? ).to eq( false )
	end


	it "knows whether the memory constraints were exceeded or not" do
		expect( opts.memory_exhausted? ).to eq( false )
	end


	it "knows whether the timer constraints were exceeded or not" do
		expect( opts.resources_exhausted? ).to eq( false )
	end


	it "can reset the resource constraints" do
		expect {
			opts.reset_resources
		}.to_not raise_error()
	end


	it "knows whether spell_guessing is enabled or not" do
		expect( opts.spell_guessing_enabled? ).to be( true ).or( be false )
	end


	it "can set the cost model type to :vdal" do
		opts.cost_model_type = :vdal
	end


	it "can set the cost model type to :corpus" do
		pending "no way to tell if the underlying library is compiled with CORPUS support or not"
		expect {
			opts.cost_model_type = :corpus
		}.to change { opts.cost_model_type }.from( :vdal ).to( :corpus )
	end


	it "doesn't allow the cost model to be set to an invalid value" do
		expect {
			opts.cost_model_type = :rafferty
		}.to raise_error( ArgumentError, /unknown cost model/i )
	end

end

