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
   * Implements Strategy::packageProducts().
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
