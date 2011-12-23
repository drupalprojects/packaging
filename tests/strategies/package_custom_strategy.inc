<?php

/**
 * @file
 * Test of custom Packaging strategy. Always creates 23 boxes.
 */


/**
 * Always creates 23 boxes, regardless of number of products in order.
 */
class PackageCustomStrategy extends PackagingStrategy {

  /**
   * Implements PackagingStrategy::getDescription().
   */
  public function getDescription() {
    return t("This strategy always creates 23 boxes, regardless of number of products in order. Its only use is to provide a custom strategy for automated testing of the Packaging module.");
  }

  /**
   * Implements PackagingStrategy::packageProducts().
   */
  public function packageProducts(array $products) {
    $package = array();

    // Creates twenty-three packages, independent of number of products.
    for ($i = 0; $i < 23; $i++) {
      $package = new Package();

      // Save the package to the array and exit.
      $packages[] = $package;
    }

    return $packages;
  }

}
