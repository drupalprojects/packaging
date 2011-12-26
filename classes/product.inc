<?php

/**
 * @file
 */

/**
 * Represents a physical product that may be put into packages.
 */
class PackagingProduct {

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


  /**
   * Constructs a new PackagingProduct object given a Drupal Commerce Product
   * entity.
   */
  public static function constructFromCommerceProduct($product) {

    $contruct = new PackagingProduct();

    return $contruct;
  }

  /**
   * Constructs a new PackagingProduct object given an array of Drupal Commerce
   * Product entities.
   */
  public static function constructFromCommerceProductMultiple(array $products) {
    $constructs = array();

    foreach ($products as $product) {
      $constructs[] = self::constructFromCommerceProduct($product);
    }

    return $constructs;
  }


  /**
   * Constructs a new PackagingProduct object given an Ubercart Product object.
   */
  public static function constructFromUbercartProduct($product) {

    // New instance.
    $contruct = new PackagingProduct();

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

  /**
   * Constructs a new Product object given an array of Ubercart Product objects.
   */
  public static function constructFromUbercartProductMultiple(array $products) {
    $constructs = array();

    foreach ($products as $product) {
      $constructs[] = self::constructFromUbercartProduct($product);
    }

    return $constructs;
  }

  /**
   * Constructor.
   *
   * WARNING. If you explicitly set the property values through the constructor
   * rather than the mutator functions, you may be introducting inconsistencies.
   * Specifically, the mutators ensure that volume always equals length x width
   * x height, as expressed in the length_units of this instance.
   *
   * @param $property_values
   *   Optional. An associative array with property names as keys and initial
   *   values as values.
   */
  public function __construct(array $property_values) {
    foreach ($property_values as $property => $value) {
      if (property_exists($this, $property)) {
        $this->$property = $value;
      }
    }
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
   * Accessor for volume property.
   */
  public function getVolume() {
    return $this->volume;
  }

  /**
   * The mutator for weight property is deliberately non-public.  If you want to
   * alter the volume you should set the length, width, and height.
   */
  protected function setVolume($volume) {
    return $this->volume = $volume;
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
    // Re-compute length, width and height with new units.
    // $this->length = packaging_length_conversion($this->length, $length_units);
    // $this->width = packaging_length_conversion($this->width, $length_units);
    // $this->height = packaging_length_conversion($this->height, $length_units);
    // Re-compute volume with new units. Volume is always expressed in terms of
    // the cube of the product length units.
    $this->volume = $this->length * $this->width * $this->height;
  }

}
