<?php

/**
 * @file
 * Packaging strategy. Packages all products in one package.
 */


/**
 * Puts all products into one package, with no restrictions.
 *
 * The "One package" stragegy is an extremely simple packaging strategy which
 * puts all products into one package. This strategy doesn't care about
 * product weights or any other metric - everything goes into one package,
 * period.
 *
 * This strategy is useful if you always ship your orders in one box.
 * Generally, that means your items are small and light enough so that any
 * quantity you can reasonably expect to sell will fit in one of your
 * standard shipping boxes.
 */
class PackageOnePackage extends PackagingStrategy {

  /**
   * Implements Strategy::packageProducts().
   */
  public function packageProducts($products) {

    // Create first and only package.
    $package = new Package();

    // Loop over products.
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the store
      // weight units.
      $item_weight = $product->getWeight() * uc_weight_conversion($product->getWeightUnits(), $this->getDefaultWeightUnits());
      $quantity = $product->getQuantity();

      // Update the package information and continue.
      $package->setQuantity($package->getQuantity() + $quantity);
      $package->setPrice($package->getPrice() + $product->getPrice() * $quantity);
      $package->setWeight($package->getWeight() + $item_weight * $quantity);
    }

    // Markup weight of package.
    $package->setShipWeight(packaging_weight_markup($package->getWeight()));

    // Save the package to the array and exit.
    $packages[] = $package;

    return $packages;
  }

}
