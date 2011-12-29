<?php

/**
 * @file
 */


/**
 * Represents a physical container that may hold products.
 *
 * $package object defined by this class contains the following members:
 *   - quantity:     Number of items in package.
 *   - price:        Value (sales price, in store currency) of items in package.
 *   - weight:       Actual weight of items in package, in store weight units.
 *   - weight_units: Taken from packaging_weight_units variable. (?)
 *   - shipweight:   Computed weight of package, including markup.
 */
class PackagingPackage {

  protected $quantity     = 0;
  protected $price        = 0.0;
  protected $weight       = 0.0;
  protected $weight_units = 'LB';
  protected $volume       = 0.0;
  protected $length_units = 'IN';
  protected $shipweight   = 0.0;

  /**
   * Array holding references to the PackagingProduct objects stored
   * in this package.
   */
  protected $products = array();


  /**
   * Accessor for quantity property.
   */
  public function getQuantity() {
    return $this->quantity;
  }

  /**
   * Mutator for quantity property.
   */
  public function setQuantity($quantity) {
    $this->quantity = $quantity;
  }

  /**
   * Accessor for price property.
   */
  public function getPrice() {
    return $this->price;
  }

  /**
   * Mutator for price property.
   */
  public function setPrice($price) {
    $this->price = $price;
  }

  /**
   * Accessor for weight property.
   */
  public function getWeight() {
    return $this->weight;
  }

  /**
   * Mutator for weight property.
   */
  public function setWeight($weight) {
    $this->weight = $weight;
  }

  /**
   * Accessor for weight_units property.
   */
  public function getWeightUnits() {
    return $this->weight_units;
  }

  /**
   * Mutator for weight_units property.
   */
  public function setWeightUnits($weight_units) {
    $this->weight_units = $weight_units;
  }

  /**
   * Accessor for shipweight property.
   */
  public function getShipWeight() {
    return $this->shipweight;
  }

  /**
   * Mutator for shipweight property.
   */
  public function setShipWeight($shipweight) {
    $this->shipweight = $shipweight;
  }

  /**
   * Accessor for volume property.
   */
  public function getVolume() {
    return $this->volume;
  }

  /**
   * Mutator for volume property.
   */
  public function setVolume($volume) {
    $this->volume = $volume;
  }

  /**
   * Accessor for length_units property.
   */
  public function getLengthUnits() {
    return $this->length_units;
  }

  /**
   * Mutator for length_units property.
   */
  public function setLengthUnits($length_units) {
    $this->length_units = $length_units;
  }

  /**
   * Accessor for products property.
   */
  public function getProducts() {
    return $this->products;
  }

  /**
   * Mutator for products property.
   */
  public function setProducts(array $products) {
    $this->products = $products;
    // Re-compute package totals.
    $this->updateTotals();
  }

  /**
   * Recomputes the package weight, price, etc. based on the current
   * list of products in the package.
   */
  protected function updateTotals() {
    foreach ($this->products as $product) {
      // Add product values to the package values.
      $quantity = $product->getQuantity();
      $this->quantity += $quantity;
      $this->price    += $quantity * $product->getPrice();
      $this->weight   += $quantity * $product->getWeight();
      $this->volume   += $quantity * $product->getVolume();
    }
  }


  /**
   * Adds a product to the package.
   */
  public function addProduct(PackagingProduct $product) {
    $this->products[] = $product;

    // Update the package values to include this new product.
    $quantity = $product->getQuantity();
    $this->quantity += $quantity;
    $this->price    += $quantity * $product->getPrice();
    $this->weight   += $quantity * $product->getWeight();
    $this->volume   += $quantity * $product->getVolume();
  }

}
