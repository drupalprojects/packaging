<?php

/**
 * @file
 */

/**
 * Represents a physical product that may be put into packages.
 */
class Product {

  protected $origin;
  protected $destination;

  protected $quantity     = 0.0;
  protected $pkg_qty      = 0.0;

  protected $price        = 0.0;
  protected $weight       = 0.0;
  protected $weight_units = 'LB';

  protected $length       = 0.0;
  protected $width        = 0.0;
  protected $height       = 0.0;
  protected $volume       = 0.0;
  protected $length_units = 'IN';


  public static function constructFromCommerceProduct($product) {

    $contruct = new Product();

    return $contruct;
  }

  public static function constructFromCommerceProductMultiple(array $products) {
    $constructs = array();

    foreach ($products as $product) {
      $constructs[] = self::constructFromCommerceProduct($product);
    }

    return $constructs;
  }


  public static function constructFromUbercartProduct($product) {

    // New instance.
    $contruct = new Product();

    // Copy properties into this instance.
    $contruct->quantity     = $product->qty;
    $contruct->pkg_qty      = $product->pkg_qty;

    $contruct->price        = $product->sell_price;
    $contruct->weight       = $product->weight;
    $contruct->weight_units = $product->weight_units;

    $contruct->length       = $product->length;
    $contruct->width        = $product->width;
    $contruct->height       = $product->height;
    $contruct->volume       = $product->length * $product->width * $product->height;
    $contruct->length_units = $product->length_units;

    return $contruct;
  }

  public static function constructFromUbercartProductMultiple(array $products) {
    $constructs = array();

    foreach ($products as $product) {
      $constructs[] = self::constructFromUbercartProduct($product);
    }

    return $constructs;
  }

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
   * Accessor for pkg_qty property.
   */
  public function getPackageQuantity() {
    return $this->pkg_qty;
  }

  /**
   * Mutator for pkg_qty property.
   */
  public function setPackageQuantity($pkg_qty) {
    $this->quantity = $pkg_qty;
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
   * Accessor for length property.
   */
  public function getLength() {
    return $this->length;
  }

  /**
   * Mutator for length property.
   */
  public function setLength($length) {
    $this->length = $length;
    // Re-compute volume
    $this->volume = $this->length * $this->width * $this->height;
  }

  /**
   * Accessor for width property.
   */
  public function getWidth() {
    return $this->width;
  }

  /**
   * Mutator for width property.
   */
  public function setWidth($width) {
    $this->width = $width;
    // Re-compute volume
    $this->volume = $this->length * $this->width * $this->height;
  }

  /**
   * Accessor for height property.
   */
  public function getHeight() {
    return $this->height;
  }

  /**
   * Mutator for height property.
   */
  public function setHeight($height) {
    $this->height = $height;
    // Re-compute volume
    $this->volume = $this->length * $this->width * $this->height;
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
    // Re-compute volume.  Volume is always expressed in terms of the cube of
    // the product weight units.
    $this->volume = $this->length * $this->width * $this->height;
  }

}
