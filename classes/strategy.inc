<?php

/**
 * @file
 * Class definition for packaging strategies.
 */


/**
 * Abstract class for packaging strategies.
 *
 * Declares a packageProducts() function which much be implemented by
 * subclasses wishing to provide their own packaging strategy. Defines
 * some utility routines.
 */
abstract class PackagingStrategy {

  /** Constant used in internal implementation to indicate unlimited weight */
  const UNLIMITED_PACKAGE_WEIGHT = -1;

  protected $maximum_weight = self::UNLIMITED_PACKAGE_WEIGHT;
  protected $weight_units = 'LB';
  protected $weight_markup_function = 'packaging_weight_markup';


  /**
   * Abstract function for packaging strategies.
   *
   * Subclasses must implement this function to provide a strategy.
   *
   * @param $products
   *   An array of nodes of type Product.
   *
   * @return
   *   An array of Package objects, each containing one or more of the products.
   */
  abstract public function packageProducts(array $products);

  /**
   * Mutator for maximum_weight property.
   */
  public function setMaximumPackageWeight($maximum_weight = self::UNLIMITED_PACKAGE_WEIGHT) {
    $this->maximum_weight = $maximum_weight;
  }

  /**
   * Accessor for maximum_weight property.
   */
  public function getMaximumPackageWeight() {
    return $this->maximum_weight;
  }

  /**
   * Compares weight against maximum allowed weight.
   *
   * @param $weight
   *   Float containing weight in default weight units.
   *
   * @return
   *   TRUE if $weight exceeds maximum allowed weight, FALSE otherwise.
   */
  public function exceedsMaximumPackageWeight($weight) {
    if ($this->maximum_weight != self::UNLIMITED_PACKAGE_WEIGHT &&
        $weight > $this->maximum_weight) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Compares weight against maximum allowed weight.
   *
   * @param $weight
   *   Float containing weight in default weight units.
   *
   * @return
   *   TRUE if $weight is less than the maximum allowed weight, FALSE otherwise.
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
    // @todo: Need to modify $maximum_weight to agree with the new units.
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
