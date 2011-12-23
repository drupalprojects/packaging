<?php

/**
 * @file
 * Packaging strategy. Packages all products in one package, break by volume.
 */


/**
 * Puts all products into packages, subject only to package maximum volume.
 *
 * The "By volume" is the equivalent of the "All-in-one" strategy, except
 * product and package volume is used instead of product and package weight.
 * "By volume" is a general-purpose packaging strategy which attempts to put
 * all products into one package, subject only to a maximum volume. When the
 * maximum volume is exceeded, a new package will be created.
 *
 * Products are added to packages one-by-one, in order of volume with the
 * largest products added first. If adding a product will exceed the package
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
class PackageByVolume extends PackagingStrategy {

  /**
   * Implements PackagingStrategy::getDescription().
   */
  public function getDescription() {
    return t("The 'By volume' is the equivalent of the 'All-in-one' strategy, except product and package volume is used instead of product and package weight.  'By volume' is a general-purpose packaging strategy which attempts to put all products into one package, subject only to a maximum volume. When the maximum volume is exceeded, a new package will be created.

Products are added to packages one-by-one, in order of volume with the largest products added first. If adding a product will exceed the package maximum weight, this strategy looks for available space in any previously-created packages and attempts to add the product to one of those.  If the product won't fit into any existing packages, a new package is created.

If your product weights are small compared to your maximum package weight this strategy will approximate an optimal packing. However, no attempt is made to optimize packing, so the number of packages returned by this strategy is not guaranteed to be the minimum possible number. This strategy mimics how a human would put products into packages, which is also not guaranteed to be optimal, but is what will occur in practice.

This strategy will always return the same results if given the same set of products.");
  }

  /**
   * Implements PackagingStrategy::packageProducts().
   */
  public function packageProducts(array $products) {

    // Loop over products.
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the store
      // weight units.
      $product->setWeight($product->getWeight() * uc_weight_conversion($product->getWeightUnits(), $this->getDefaultWeightUnits()));
      $product->setWeightUnits($this->getDefaultWeightUnits());
    }

    // Sort products from heaviest to lightest to make packaging deterministic.
    // Products have to all have the same weight units for a valid comparison!
    usort($products, 'PackageByVolume::compareWeights');

    // Create first package.
    $packages = array();
    $package  = new Package();
    $package->setWeightUnits($this->getDefaultWeightUnits());

    // Loop over products.
    foreach ($products as $product) {
      // Get product weight.
      $product_weight = $product->getWeight();

      if ($this->exceedsMaximumPackageWeight($product_weight)) {
        // This product is too heavy to ship - quit with error.
        return array();
      }

      // Loop over quantity of each product.
      for ($item = 0; $item < $product->getQuantity(); $item++) {
        // Test to see if putting this item into the current package will put
        // us over the package weight limit.
        if ($this->lessThanMaximumPackageWeight($package->getWeight() + $product_weight)) {
          // No?  Then update the package information and continue.
          $package->setQuantity($package->getQuantity() + 1);
          $package->setPrice($package->getPrice() + $product->getPrice());
          $package->setWeight($package->getWeight() + $product_weight);
        }
        else {
          // If weight >= maximum allowed weight, try to fit this item into one
          // of the previously-filled packages. If this item is small it might
          // fit into the available space rather than needing a new package.
          $found_space = FALSE;
          foreach ($packages as &$filled_package) {
            // Will it fit?
            if ($this->lessThanMaximumPackageWeight($filled_package->getWeight() + $product_weight)) {
              // Yes, update values for previously-filled package.
              $found_space = TRUE;
              $filled_package->setQuantity($filled_package->getQuantity() + 1);
              $filled_package->setPrice($filled_package->getPrice() + $product->getPrice());
              $filled_package->setWeight($filled_package->getWeight() + $product_weight);
              // Because we've added a product, we need to adjust the weight
              // markup weight of this previously-filled package.
              $filled_package->setShipWeight(packaging_weight_markup($filled_package->getWeight()));
              // Found some free room for this item.
              break;
            }
          }
          if (!$found_space) {
            // If we reach here, the item won't fit in the current package and
            // won't fit in a previously-filled package, so we need to close off
            // the current package and start a new one.

            // Markup weight of current package.
            $package->setShipWeight(packaging_weight_markup($package->getWeight()));

            // Save current package to the array.
            $packages[] = $package;

            // Put this item into a new package.
            $package = new Package();
            $package->setQuantity(1);
            $package->setPrice($product->getPrice());
            $package->setWeight($product_weight);
            $package->setWeightUnits($this->getDefaultWeightUnits());
          }
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
   * Sort callback. Sorts array of Product objects by weight from high to low.
   */
  protected static function compareWeights(Product $a, Product $b) {
    $weight_a = $a->getWeight();
    $weight_b = $b->getWeight();

    if ($a == $b) {
      return 0;
    }
    // Reverse order.
    return ($weight_a < $weight_b) ? 1 : -1;
  }

}
