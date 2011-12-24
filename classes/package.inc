<?php

/**
 * @file
 */


/**
 * $package object defined by this class contains the following members:
 *   - quantity:     Number of items in package.
 *   - price:        Value (sales price, in store currency) of items in package.
 *   - weight:       Actual weight of items in package, in store weight units.
 *   - weight_units: Set to store default, taken from uc_weight_unit variable.
 *   - shipweight:   Computed weight of package, including markup.
 */
class PackagingPackage {

  protected $quantity     = 0;
  protected $price        = 0.0;
  protected $weight       = 0.0;
  protected $weight_units = 'LB';
  protected $shipweight   = 0.0;


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
   * Mutator for weight property.
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
   * Mutator for weight property.
   */
  public function setShipWeight($shipweight) {
    $this->shipweight = $shipweight;
  }

}
