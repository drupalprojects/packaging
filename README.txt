Motivation
==========
Packaging has until now be implemented independently by each and every
different shipping quote module.

Abstracting packaging out into its own module accomplishes four things:

1) Code duplication is eliminated. No longer will USPS, UPS, FedEX,
   Canada Post, etc. all have their own slightly-different implementations.
2) Packaging is done uniformly between different quote methods. "All-in-one"
   will now package an order exactly the same way for USPS as it does for UPS.
3) Quote methods that don't currently use packaging, such as uc_flatrate, now
   have the option to use it. So uc_flatrate may now charge a flatrate per
   package, if for example the order ships to more than one destination or
   originates from more than one warehouse.
4) Packaging strategies are extensible using a plugin mechanism.  If the
   built-in packaging methods don't suit you, it's easy to write your own and
   share your own with others.

Defining your own strategy
==========================

1) Create a new module.
2) Module should contain a subclass PackagingStrategy.
3) Implement the strategy() function in your subclass.
4) Register your strategy using hook_packaging_strategy().
5) In the module that uses strategies (e.g. Ubercart or Commerce),
   set up a rule to select your strategy under the appropriate conditions.

