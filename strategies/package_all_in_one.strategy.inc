<?php

/**
 * @file
 * Packaging strategy. Packages all products in one package, break by weight.
 */


/**
 * Puts all products into packages, subject only to package maximum weight.
 *
 * The "All-in-one" stragegy is a general-purpose packaging strategy which
 * attempts to put all products into one package, subject only to a maximum
 * weight. When the maximum weight is exceeded, more than one package will
 * be created.
 *
 * Products are added to packages one-by-one, in the order they are passed to
 * the strategy method. If adding a product will exceed the package maximum
 * weight, this strategy looks for available space in any previously-created
 * packages and attempts to add the product to one of those. If the product
 * won't fit into any existing packages, a new package is created.
 *
 * If your product weights are small compared to your maximum package weight
 * this strategy will approximate an optimal packing. However, no attempt is
 * made to optimize packing, so the number of packages returned by this
 * strategy is not guaranteed to be the minimum possible number. This strategy
 * mimics how a human would put products into packages, which is also not
 * guaranteed to be optimal, but is what will occur in practice.
 */
class PackageAllInOne extends PackagingStrategy {

  /**
   * Implements Strategy::strategy().
   */
  public function strategy($products) {

    // Create first package.
    $package = new Package();

    // Loop over products.
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the store
      // weight units.
      $item_weight = $product->getWeight() * uc_weight_conversion($product->getWeightUnits(), $this->getDefaultWeightUnits());

      if ($this->exceedsMaximumPackageWeight($item_weight)) {
        // This product is too heavy to ship - quit with error.
        return array();
      }

      // Loop over quantity of each product.
      for ($item = 0; $item < $product->getQuantity(); $item++) {
        // Test to see if putting this item into the current package will put
        // us over the package weight limit.
        if ($this->lessThanMaximumPackageWeight($package->getWeight() + $item_weight)) {
          // No?  Then update the package information and continue.
          $package->setQuantity($package->getQuantity() + 1);
          $package->setPrice($package->getPrice() + $product->getPrice());
          $package->setWeight($package->getWeight() + $item_weight);
        }
        else {
          // If weight >= maximum allowed weight, save current package to
          // array and start a new package:

          // First, markup weight of current package.
          $package->setShipWeight(packaging_weight_markup($package->getWeight()));

          // Second, save current package to array.
          $packages[] = $package;

          // Third, try to fit this item into one of the previously-filled
          // packages?  If this item is small it might fit into the available
          // space rather than needing a new package.
          foreach ($packages as $filled_package) {
            if ($this->lessThanMaximumPackageWeight($filled_package->getWeight() + $item_weight)) {
              $filled_package->setQuantity($filled_package->getQuantity() + 1);
              $filled_package->setPrice($filled_package->getPrice() + $product->getPrice());
              $filled_package->setWeight($filled_package->getWeight() + $item_weight);
            }
            else {
              // If all else fails, start a new package.
              $package = new Package();
              $package->setQuantity(1);
              $package->setPrice($product->getPrice());
              $package->setWeight($item_weight);
              $package->setWeightUnits($this->getDefaultWeightUnits());
            }
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

}
