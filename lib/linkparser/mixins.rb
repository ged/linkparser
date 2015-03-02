# -*- ruby -*-
#encoding: utf-8

module LinkParser

	### Functions for marking methods as deprecated.
	module DeprecationUtilities

		# Hash of which warnings have already been output
		$lp_deprecation_warnings = {}

		### Make a wrapper for a deprecated method. The wrapper will print a deprecation warning
		### to STDERR, and then call the method with the same name prefixed with an underscore.
		def deprecated_method( *names )
			names.each do |name|
				method_body = lambda do |*args|
					source = caller( 1 ).first
					if $lp_deprecation_warnings.key?( source )
						$lp_deprecation_warnings[ source ] += 1
					else
						$lp_deprecation_warnings[ source ] = 1
						warn "Use of deprecated method %p from %s." % [ name, source ]
					end

					return self.method( "_#{name}" ).call( *args )
				end

				# Install the wrapper after aliasing away the old method
				alias_method( "_#{name}", name )
				remove_method( name )
				define_method( name, &method_body )
			end
		end

	end # module DeprecationUtilities

end # module LinkParser
