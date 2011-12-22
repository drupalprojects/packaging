About
=====
This module provides algorithms for splitting shippable orders into physical
packages. The intent is that shipping quotes modules will use these
algorithms in lieu of coding their own packaging routines. New algorithms may
easily be provided using the plugin architecture of this module.


Motivation
==========
Packaging has until now has been implemented independently by each and every
different shipping quote module.

Abstracting packaging out into its own module accomplishes five things:

1) Code duplication is eliminated. No longer will USPS, UPS, FedEX,
   Canada Post, etc. all have their own slightly-different implementations.
2) Packaging is done uniformly between different quote methods. "All-in-one"
   will now package an order exactly the same way for USPS as it does for UPS.
3) Quote methods that don't currently use packaging, such as uc_flatrate, now
   have the option to use it. So uc_flatrate may now charge a flatrate per
   package, if for example the order ships to more than one destination or
   originates from more than one warehouse.
4) Packaging strategies are extensible using a plugin mechanism. If the
   built-in packaging methods don't suit you, it's easy to write your own and
   share your own with others.
5) Architecture enables development of new features. Packaging strategies can
   be computationally difficult: by abstracting out the packaging strategy this
   module provides a simple mechanism to develop and test new features.



Default strategies
==================
package_all_in_one.inc
  Roughly corresponds to the Ubercart "all-in-one" packaging method. Code was
  taken from uc_fedex. Unifies the two different implementations provided by
  uc_ups and uc_usps and fixes many bugs with both implementations.
  Specifically, this method properly handles varying product weight units,
  makes better use of available weight, allows configuration of maximum allowed
  weight. The results are independent of the order of products in the cart.
  
package_each_in_own.inc
  Corresponds to the Ubercart "each-in-own" packaging method.

package_one_package.inc
  For backwards compatibility with shipping quote methods that didn't do
  packaging, like uc_weightquote and uc_flatrate.

package_average_weight.inc
  Number of boxes is determined by order total weight divided by package
  maximum weight.

package_by_volume.inc
  Same as the all-in-one strategy, but using volume rather than weight as the
  way to break an order into packages.

package_average_volume.inc
  Number of boxes is determined by order total volume divided by package
  maximum volume.

ALL the default strategies work with multiple origin addresses and multiple
destination addresses. ALL the default strategies may be applied to the entire
order or to just a subset of the order's product, allowing you to "mix and
match" packaging strategies on a per-product basis. This is useful when you
have certain products that must ship separately, must be drop-shipped, or
must be shipped in case lots.


Defining your own strategy
==========================

An example of a module that creates a new packaging strategy,
packaging_test.module, may be found in the tests subdirectory. Use this
working example as a guide when writing your own module.  Here are the
minimum necessary steps:

1) Create a new module for your strategy.
2) Module must implement two hooks:
   a) hook_test_ctools_plugin_directory() - indicates where your module's
      plugings can be found. Typically this would be in a subdirectory under
      your module's main directory.
   b) hook_packaging_strategy() - declares the strategy name(s) and handler
      class(es) for any packaging strategies implemented by your module.
3) Create an include file in your plugin directory with defines a subclass
   of PackagingStrategy.
4) Implement the packageProducts() function in your subclass.
5) In the module that uses strategies (e.g. Ubercart or Commerce),
   set up a rule to select your strategy under the appropriate conditions.


Troubleshooting
===============
Extensive tests are provided for each of the default strategies to ensure that
they work under all possible combinations of settings. If you believe you have
found a bug, the first step is to examine the test cases and make sure your
code operates the same way as the test cases. Bug reports that provide tests
to demonstrate the reported error will be given first priority in the issue
queue.
