<?php

/**
 * @file
 * Class and interface definitions for packaging strategies.
 */


/**
 * Interface to define a strategy.
 */
interface Strategy {

  /**
   * @param $products
   *   An array of nodes of type Product.
   *
   * @return
   *   An array of Package objects, each containing one or more of the products.
   */
  public function strategy($products);
}


/**
 * Abstract class for packaging strategies. Implements the Strategy interface
 * and defines some useful utility routines.
 */
abstract class PackagingStrategy implements Strategy {

  /** Constant used in internal implementation to indicate unlimited weight */
  const UNLIMITED_PACKAGE_WEIGHT = -1;

  protected $maximum_weight = self::UNLIMITED_PACKAGE_WEIGHT;
  protected $weight_units = 'LB';
  protected $weight_markup_function = 'packaging_weight_markup';


  /**
   *
   */
  public function setMaximumPackageWeight($maximum_weight = self::UNLIMITED_PACKAGE_WEIGHT) {
    $this->maximum_weight = $maximum_weight;
  }

  /**
   *
   */
  public function exceedsMaximumPackageWeight($weight) {
    if ($this->maximum_weight != self::UNLIMITED_PACKAGE_WEIGHT &&
        $weight > $this->maximum_weight) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   */
  public function lessThanMaximumPackageWeight($weight) {
    if ($this->maximum_weight == self::UNLIMITED_PACKAGE_WEIGHT ||
        $weight < $this->maximum_weight) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Accessor for weight_units property.
   */
  public function setDefaultWeightUnits($weight_units = 'LB') {
    $this->weight_units = $weight_units;
  }

  /**
   * Accessor for weight_units property.
   */
  public function getDefaultWeightUnits() {
    return $this->weight_units;
  }

  /**
   * A function may be specified to modify the package weight after packaging
   * is completed. This is intended to compensate for the weight of packaging
   * materials.
   */
  public function setWeightMarkupFunction($weight_markup_function) {
    $this->weight_markup_function = $weight_markup_function;
  }
}
