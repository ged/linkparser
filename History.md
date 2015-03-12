## v2.0.0 [2015-03-02] Michael Granger <ged@FaerieMUD.org>

Updated for the latest Ruby and link-grammar.

NOTE: This release breaks the API of previous releases. In the 5.x
release of the upstream library (link-grammar) a bunch of
long-deprecated functions were removed, so it was no longer possible
to maintain backward-compatibility.

The 1.x version of LinkParser should continue to work fine for
pre-5.x versions of link-grammar, and it will continue to be
maintained for the forseeable future.


## v1.1.4 [2012-01-31] Michael Granger <ged@FaerieMUD.org>

- Packaging fixes, dependency update


## v1.1.3 [2011-05-16] Michael Granger <ged@FaerieMUD.org>

- Use a relative path for path constants (Alessandro Berardi), update
  deps


## v1.1.2 [2011-05-12] Michael Granger <ged@FaerieMUD.org>

- Fixes package configuration options concatenation bug (see
  http://goo.gl/G49Y8) (Alessandro Berardi <berardialessandro@gmail.com>)
- De-Yard, build cleanup, added gem-testers support
- Removed unnecessary (and non-existant on Ruby < 1.9) include.
- Fixing some shadowed variables for 1.9.2.
- Avoid circular requires.


## v1.1.1 [2010-12-30] Michael Granger <ged@FaerieMUD.org>

* Updated for link-grammar 4.7.1.
* Fixed some specs that were doing nothing, using old RSpec syntax, etc.
* Converted to Hoe.


## v1.1.0 [2010-11-30] Michael Granger <ged@FaerieMUD.org>

* Updated to support link-grammar 4.7.0. Note that this breaks compatibility with
  earlier versions, as the model for sentences with conjunctions has changed
  significantly.
* Use pkgconfig if available.
* Various memory-management and 1.9.2 fixes.


## v1.0.6 [2009-10-16] Michael Granger <ged@FaerieMUD.org>

[TODO]

