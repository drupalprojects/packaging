<?php

/**
 * @file
 * Packaging strategy. Creates identical packages based on product averages.
 */


/**
 * Puts all products into packages, subject only to package maximum weight.
 *
 * The "All-in-one" strategy is a general-purpose packaging strategy which
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
class PackageAverageVolume extends PackagingStrategy {

  /**
   * Implements Strategy::packageProducts().
   */
  public function packageProducts(array $products) {

    // Create product aggregate for averaging.
    $product_aggregate = new Product();
    foreach ($products as $product) {
      // Get item weight. Weight units are set on a per-product basis, so we
      // convert as necessary in order to perform all calculations in the store
      // weight units.
      $product_aggregate->setQuantity($product_aggregate->getQuantity() + $product->getQuantity());
      $product_aggregate->setPrice($product_aggregate->getPrice() + $product->getPrice());
      $product_aggregate->setWeight($product_aggregate->getWeight() + $product->getQuantity() * $product->getWeight() * uc_weight_conversion($product->getWeightUnits(), $this->getDefaultWeightUnits()));
    }

    // Calculate the number of packages we will need.
    if ($this->getMaximumPackageWeight() == PackagingStrategy::UNLIMITED_PACKAGE_WEIGHT) {
      $num_packages = 1;
    }
    else {
      $num_packages = ceil($product_aggregate->getWeight() / $this->getMaximumPackageWeight());
    }

    $average_quantity = $product_aggregate->getQuantity() / $num_packages;
    $average_price = $product_aggregate->getPrice() / $num_packages;
    $average_weight = $product_aggregate->getWeight() / $num_packages;

    $packages = array();
    for ($i = 0; $i < $num_packages; $i++) {
      // Create package.
      $package = new Package();

      // Set package values to the average values.
      $package->setQuantity($average_quantity);
      $package->setPrice($average_price);
      $package->setWeight($average_weight);
      $package->setWeightUnits($this->getDefaultWeightUnits());

      // Markup weight of package.
      $package->setShipWeight(packaging_weight_markup($package->getWeight()));

      // Save the package to the array.
      $packages[] = $package;
    }

    return $packages;
  }

}
