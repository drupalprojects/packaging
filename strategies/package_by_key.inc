<?php

/**
 * @file
 * Packaging strategy. Puts products with identical keys into same package.
 */


/**
 * Puts products with identical keys into same package.
 *
 * The "By key" strategy uses certain keys set for each product to determine
 * how to package an order. Product keys are simply properties of the
 * PackagingProduct object which have been designated as keys by invoking
 * PackagingContext::designateKeys(array $keys). You may pass an array of
 * property names to that method to identify which properties are keys.
 *
 * Once the keys have been designated, this strategy will look for products
 * which have a common set of keys and put those products in one package.
 *
 * A typical use case would be for products that are drop-shipped from a
 * location other than your store. In this use case, products may have
 * different origin addresses, but may be destined for the same delivery
 * address. This physically requires more than one package. Designating both
 * origin and destination addresses as keys ensures that there is one package
 * for each combination in the set {origin, destination}. For example, with
 * two origin addresses and one destination address you will get two boxes.
 * for two origin addresses and two destination addresses you may get up to
 * four boxes.
 *
 * Another use case is to use a taxonomy term as a key. This strategy will then
 * package all products with the same term in the same package. This is useful
 * when you have perisable products, for instance, that may need to be shipped
 * together via an Express method, or with special handling.
 *
 * WARNING: If you designate too many keys, this routine will blow up under the
 * combinatorics. If there are n keys designated, each with m_{i} different
 * values, the number of combinations is \prod_{i=1}^{n} m_{i} which may be
 * very large.
 */
class PackageByKey extends PackagingStrategy {

  /**
   * Implements PackagingStrategy::getDescription().
   */
  public function getDescription() {
    return t("The 'By key' strategy uses certain keys set for each product to determine how to package an order. Product keys are simply properties of the PackagingProduct object which have been designated as keys by invoking PackagingContext::designateKeys(array \$keys). You may pass an array of property names to that method to identify which properties are keys.

Once the keys have been designated, this strategy will look for products which have a common set of keys and put those products in one package.");
  }

  /**
   * Implements PackagingStrategy::packageProducts().
   */
  public function packageProducts(PackagingContext $context, array $products) {

    // Weight units are set on a per-product basis, so convert all products
    // to default weight units before performing any calculations.
    foreach ($products as $product) {
      $product->setWeight($product->getWeight() * packaging_weight_conversion($product->getWeightUnits(), $context->getDefaultWeightUnits()));
      $product->setWeightUnits($context->getDefaultWeightUnits());
    }

    $hashtable = array();
    $keynames = PackagingContext::getKeys();

//debug($products);
    // Loop over products to build hashtable. Each entry in the hashtable
    // represents a package. Products with the same hash value will be in
    // the same package.
    foreach ($products as $index => $product) {
      // Compute a hash value using all the designated keys.
      // Products with equal key values will have equal hash values.
      $hash = '';
      foreach ($keynames as $key) {
        if (!property_exists($product, $key)) {
          // This product doesn't have one of the required keys, so we quit.
          return array();
        }
        $hash .= $product->$key;
      }
      // Save product index in hashtable.
      $hashtable[$hash][] = $index;
    }

//debug($hashtable);
    // Loop over hashtable entries to build PackagingPackage objects.
    // Each hashtable entry corresponds to one package.
    $packages = array();
    foreach ($hashtable as $product_list) {
      // Create package object.
      $package  = new PackagingPackage();
      $package->setWeightUnits($context->getDefaultWeightUnits());

      // Loop over product indexes stored in the hashtable.
      foreach ($product_list as $index) {
        // Add product values to the package values.
        $quantity = $products[$index]->getQuantity();
        $package->setQuantity($package->getQuantity() + $quantity);
        $package->setPrice($package->getPrice() + $quantity * $products[$index]->getPrice());
        $package->setWeight($package->getWeight() + $quantity * $products[$index]->getWeight()
);
        $package->setWeightUnits($context->getDefaultWeightUnits());
      }

      // Copy designated product keys into the package object so we have a
      // reference of what's in there.
      foreach ($keynames as $key) {
        $package->$key = $products[reset($product_list)]->$key;
      }

      // Markup weight of package.
      $package->setShipWeight(packaging_weight_markup($package->getWeight()));
//debug($package);

      // Save the package to the array.
      $packages[] = $package;
    }

    return $packages;
  }

}
