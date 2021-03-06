<?php

/**
 * @file
 * Packaging strategy. Creates identical packages based on product averages.
 */


/**
 * Puts all products into packages, subject only to package maximum weight.
 *
 * The "Average weight" strategy computes the number of packages needed by
 * dividing the total weight of all products by the maximum allowed package
 * weight. The resulting packages are assigned identical weights, prices, etc.
 * so as to simulate uniform distribution of products amongst the packages.
 */
class PackageAverageWeight implements PackagingStrategy {

  /**
   * Implements PackagingStrategy::getDescription().
   */
  public function getDescription() {
    return t("The 'Average weight' strategy computes the number of packages needed by dividing the total weight of all products by the maximum allowed package weight. The resulting packages are assigned identical weights, prices, etc.  so as to simulate uniform distribution of products amongst the packages.");
  }

  /**
   * Implements PackagingStrategy::packageProducts().
   */
  public function packageProducts(PackagingContext $context, array $products) {

    // Create product aggregate for averaging.
    $product_aggregate = new PackagingProduct();
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the
      // default weight units.
      $product_aggregate->setQuantity($product_aggregate->getQuantity() + $product->getQuantity());
      $product_aggregate->setPrice($product_aggregate->getPrice() + $product->getPrice());
      $product_aggregate->setWeight($product_aggregate->getWeight() + $product->getQuantity() * $product->getWeight() * packaging_weight_conversion($product->getWeightUnits(), $context->getDefaultWeightUnits()));
    }

    // Calculate the number of packages we will need.
    if ($context->getMaximumPackageWeight() == PackagingContext::UNLIMITED_PACKAGE_WEIGHT) {
      $num_packages = 1;
    }
    else {
      $num_packages = ceil($product_aggregate->getWeight() / $context->getMaximumPackageWeight());
    }

    $average_quantity = $product_aggregate->getQuantity() / $num_packages;
    $average_price = $product_aggregate->getPrice() / $num_packages;
    $average_weight = $product_aggregate->getWeight() / $num_packages;

    $packages = array();
    for ($i = 0; $i < $num_packages; $i++) {
      // Create package.
      $package = new PackagingPackage();

      // Set package values to the average values.

      // Should use "Worst fit" strategy here to allocate products
      // to packages as evenly as possible.
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
