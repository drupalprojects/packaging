<?php

/**
 * @file
 * Test of custom Packaging strategy. Always creates 23 boxes.
 */


/**
 * Always creates 23 boxes, regardless of number of products in order.
 *
 * This is a clone of PackageAverageWeight, with the number of packages
 * hardwired to 23.
 */
class PackageCustomStrategy implements PackagingStrategy {

  /**
   * Implements PackagingStrategy::getDescription().
   */
  public function getDescription() {
    return t("This strategy always creates 23 boxes, regardless of number of products in order. Its only use is to provide a custom strategy for automated testing of the Packaging module.");
  }

  /**
   * Implements PackagingStrategy::packageProducts().
   */
  public function packageProducts(PackagingContext $context, array $products) {
    // Creates twenty-three packages, independent of number of products.
    $num_packages = 23;

    // Create product aggregate for averaging.
    $product_aggregate = new PackagingProduct();
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the store
      // weight units.
      $product_aggregate->setQuantity($product_aggregate->getQuantity() + $product->getQuantity());
      $product_aggregate->setPrice($product_aggregate->getPrice() + $product->getPrice());
      $product_aggregate->setWeight($product_aggregate->getWeight() + $product->getQuantity() * $product->getWeight() * packaging_weight_conversion($product->getWeightUnits(), $context->getDefaultWeightUnits()));
    }

    $average_quantity = $product_aggregate->getQuantity() / $num_packages;
    $average_price = $product_aggregate->getPrice() / $num_packages;
    $average_weight = $product_aggregate->getWeight() / $num_packages;

    $packages = array();
    for ($id = 0; $id < $num_packages; $id++) {
      // Create package.
      $package = new PackagingPackage();

      // Set package values to the average values.
      $package->setQuantity($average_quantity);
      $package->setPrice($average_price);
      $package->setWeight($average_weight);
      $package->setWeightUnits($context->getDefaultWeightUnits());

      // Markup weight of package.
      $package->setShipWeight(packaging_weight_markup($package->getWeight()));

      // Save the package to the array.
      $packages[] = $package;
    }

    return $packages;
  }

}
