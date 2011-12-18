<?php

/**
 * @file
 * Packaging strategy. Packages each product line item in its own package.
 */


/**
 * Puts each product line item into its own package, subject only to package
 * quantity restriction for that line item.
 *
 * The "Each-in-own" stragegy is a general-purpose packaging strategy which
 * puts each product line item into its own package. This strategy is intended
 * for products which come pre-packaged into shippable cases.  The product's
 * package quantity property restricts the number in each package, so if more
 * than that number are in the order, multiple packages will be sent.  This is
 * useful for products that come in cases, for example, since exceeding the
 * case quantity means you will have to ship more than one case.
 *
 * Products are added to packages one-by-one, in the order they are passed to
 * the strategy method. If the product quantity will exceed the product package
 * quantity property, this strategy will create a new package. Likewise, a new
 * package will be created for each individual product line item.
 */
class PackageEachInOwn extends PackagingStrategy {

  /**
   * Implements Strategy::strategy().
   */
  public function strategy($products) {
    // Each product line item in its own package, subject only to pkg_qty.

    // Loop over products.
    foreach ($products as $product) {
      // If pkg_qty == 0 we assume no limit on package quantity.
      if (!$product->getPackageQuantity()) {
        // Put all of this product line item into one package.
        $product->setPackageQuantity($product->getQuantity());
      }
      // Calculate number of full packages.
      $num_of_pkgs = (int) ($product->getQuantity() / $product->getPackageQuantity());
      if ($num_of_pkgs) {
        // Full packages
        for ($i = 0; $i < $num_of_pkgs; $i++) {
          // Create full packages.
          $package = new Package();
          $package->setQuantity($product->getPackageQuantity());
          $package->setPrice($product->getPrice()   * $product->getPackageQuantity());
          $package->setWeight($product->getWeight() * $product->getPackageQuantity());
          $package->setWeightUnits($this->getDefaultWeightUnits());

          // Markup weight on a per-package basis.
          $package->setShipWeight(packaging_weight_markup($package->getWeight()));

          // Save current package to array.
          $packages[] = $package;
        }
      }

      // Deal with the remaining partially-full package.
      $remaining_qty = $product->getQuantity() % $product->getPackageQuantity();
      if ($remaining_qty) {
        // Create partially-full packages.
        $package = new Package();
        $package->setQuantity($remaining_qty);
        $package->setPrice($product->getPrice()  * $remaining_qty);
        $package->setWeight($product->getWeight() * $remaining_qty);
        $package->setWeightUnits($this->getDefaultWeightUnits());

        // Markup weight on a per-package basis.
        $package->setShipWeight(packaging_weight_markup($package->getWeight()));

        // Save package to array.
        $packages[] = $package;
      }
    }

    return $packages;
  }

}
