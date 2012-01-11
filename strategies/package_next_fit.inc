<?php

/**
 * @file
 * Packaging strategy. Packages all products in one package, break by weight.
 */


/**
 * Puts all products into packages, subject only to package maximum weight.
 *
 * The "All-in-one" strategy is a general-purpose packaging strategy which
 * attempts to put all products into one package, subject only to a maximum
 * weight. When the maximum weight is exceeded, a new package will be created.
 *
 * Products are added to packages one-by-one, in order of weight with the
 * heaviest products added first. If adding a product will exceed the package
 * maximum weight, this strategy looks for available space in any
 * previously-created packages and attempts to add the product to one of those.
 * If the product won't fit into any existing packages, a new package is
 * created.
 *
 * If your product weights are small compared to your maximum package weight
 * this strategy will approximate an optimal packing. However, no attempt is
 * made to optimize packing, so the number of packages returned by this
 * strategy is not guaranteed to be the minimum possible number. This strategy
 * mimics how a human would put products into packages, which is also not
 * guaranteed to be optimal, but is what will occur in practice.
 *
 * This strategy will always return the same results if given the same set of
 * products.
 */
class PackageNextFit implements PackagingStrategy {

  /**
   * Implements PackagingStrategy::getDescription().
   */
  public function getDescription() {
    return t("The 'All-in-one' strategy is a general-purpose packaging strategy which attempts to put all products into one package, subject only to a maximum weight. When the maximum weight is exceeded, a new package will be created.

Products are added to packages one-by-one, in order of weight with the heaviest products added first. If adding a product will exceed the package maximum weight, this strategy looks for available space in any previously-created packages and attempts to add the product to one of those.  If the product won't fit into any existing packages, a new package is created.

If your product weights are small compared to your maximum package weight this strategy will approximate an optimal packing. However, no attempt is made to optimize packing, so the number of packages returned by this strategy is not guaranteed to be the minimum possible number. This strategy mimics how a human would put products into packages, which is also not guaranteed to be optimal, but is what will occur in practice.

This strategy will always return the same results if given the same set of products.");
  }

  /**
   * Implements PackagingStrategy::packageProducts().
   */
  public function packageProducts(PackagingContext $context, array $products) {

    // Loop over products.
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the
      // default weight units.
      $product->setWeight($product->getWeight() * packaging_weight_conversion($product->getWeightUnits(), $context->getDefaultWeightUnits()));
      $product->setWeightUnits($context->getDefaultWeightUnits());
    }

    // Sort products from heaviest to lightest to make packaging deterministic.
    // Products have to all have the same weight units for a valid comparison!
    usort($products, 'PackageNextFit::compareWeights');

    // Create first package.
    $packages = array();
    $package  = new PackagingPackage();
    $package->setWeightUnits($context->getDefaultWeightUnits());

    // Loop over products.
    foreach ($products as $product) {
      // Get product weight.
      $product_weight = $product->getWeight();

      if ($context->exceedsMaximumPackageWeight($product_weight)) {
        // This product is too heavy to ship - quit with error.
        return array();
      }

      // Loop over quantity of each product.
      for ($item = 0; $item < $product->getQuantity(); $item++) {

        // Test to see if putting this item into the current package will put
        // us over the package weight limit.
        if (!$context->exceedsMaximumPackageWeight($package->getWeight() + $product_weight)) {
          // No?  Then update the package information and continue.
          $package->setQuantity($package->getQuantity() + 1);
          $package->setPrice($package->getPrice() + $product->getPrice());
          $package->setWeight($package->getWeight() + $product_weight);
        }
        else {
          // If weight > maximum allowed weight, save current package to
          // array and start a new package.

          // First, markup weight of current package.
          $package->setShipWeight(packaging_weight_markup($package->getWeight()));

          // Second, save current package to array.
          $packages[] = $package;

          // Third, start a new package.
          $package = new PackagingPackage();
          $package->setQuantity(1);
          $package->setPrice($product->getPrice());
          $package->setWeight($product_weight);
          $package->setWeightUnits($context->getDefaultWeightUnits());
        }
      }


    }

    // No more products left to package.
    // Take care of the partially-filled package we were working on.

    // First, markup weight of partially-filled package.
    $package->setShipWeight(packaging_weight_markup($package->getWeight()));

    // Second, save the partially-filled package to the array and exit.
    $packages[] = $package;

    return $packages;
  }

  /**
   * Sort callback. Sorts array of PackagingProduct objects by weight from high to low.
   */
  protected static function compareWeights(PackagingProduct $a, PackagingProduct $b) {
    $weight_a = $a->getWeight();
    $weight_b = $b->getWeight();

    if ($weight_a == $weight_b) {
      return 0;
    }
    // Reverse order.
    return ($weight_a < $weight_b) ? 1 : -1;
  }

}
