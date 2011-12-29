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
  public function packageProducts(PackagingContext $context, array $products) {

    // Loop over products.
    foreach ($products as $product) {
      // Get item dimensions. Length units are set on a per-product basis, so
      // we convert as necessary in order to perform all calculations in the
      // default length units.
      $dimensions = $product->getDimensions();
      foreach ($dimensions as &$dimension) {
        $dimension *= packaging_length_conversion($product->getLengthUnits(), $context->getDefaultLengthUnits());
      }
      // Do this before we set dimensions!!
      // setDimensions() should rescale all dimensions automatically.
      $product->setLengthUnits($context->getDefaultLengthUnits());
      $product->setDimensions($dimensions);
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the
      // default weight units.
      $product->setWeight($product->getWeight() * packaging_weight_conversion($product->getWeightUnits(), $context->getDefaultWeightUnits()));
      $product->setWeightUnits($context->getDefaultWeightUnits());
    }

    // Sort products from heaviest to lightest to make packaging deterministic.
    // Products have to all have the same weight units for a valid comparison!
    usort($products, 'PackageByVolume::compareVolumes');

    // Create first package.
    $packages = array();
    $package  = new PackagingPackage();
    $package->setWeightUnits($context->getDefaultWeightUnits());
    $package->setLengthUnits($context->getDefaultLengthUnits());

    // Loop over products.
    foreach ($products as $product) {
      // Get product volume.
      $product_volume = $product->getVolume();

      if ($context->exceedsMaximumPackageVolume($product_volume)) {
        // This product is too large to ship - quit with error.
        return array();
      }

      // Loop over quantity of each product.
      for ($item = 0; $item < $product->getQuantity(); $item++) {
        // Test to see if putting this item into the current package will put
        // us over the package volume limit.
        if ($context->lessThanMaximumPackageVolume($package->getVolume() + $product_volume)) {
          // No?  Then update the package information and continue.
          $package->setQuantity($package->getQuantity() + 1);
          $package->setPrice($package->getPrice() + $product->getPrice());
          $package->setWeight($package->getWeight() + $product->getWeight());
          $package->setVolume($package->getVolume() + $product_volume);
        }
        else {
          // If volume >= maximum allowed volume, try to fit this item into one
          // of the previously-filled packages. If this item is small it might
          // fit into the available space rather than needing a new package.
          $found_space = FALSE;
          foreach ($packages as &$filled_package) {
            // Will it fit?
            if ($context->lessThanMaximumPackageVolume($filled_package->getVolume() + $product_volume)) {
              // Yes, update values for previously-filled package.
              $found_space = TRUE;
              $filled_package->setQuantity($filled_package->getQuantity() + 1);
              $filled_package->setPrice($filled_package->getPrice() + $product->getPrice());
              $filled_package->setWeight($filled_package->getWeight() + $product->getWeight());
              $filled_package->setVolume($filled_package->getVolume() + $product_volume);
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
            $package = new PackagingPackage();
            $package->setQuantity(1);
            $package->setPrice($product->getPrice());
            $package->setWeight($product->getWeight());
            $package->setWeightUnits($context->getDefaultWeightUnits());
            $package->setVolume($product_volume);
            $package->setLengthUnits($context->getDefaultLengthUnits());
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
   * Sort callback. Sorts array of PackagingProduct objects by weight from high to low.
   */
  protected static function compareVolumes(PackagingProduct $a, PackagingProduct $b) {
    $volume_a = $a->getVolume();
    $volume_b = $b->getVolume();

    if ($volume_a == $volume_b) {
      return 0;
    }
    // Reverse order.
    return ($volume_a < $volume_b) ? 1 : -1;
  }

}
