<?php

/**
 * @file
 * Functions needed by shipping quotes modules to administer packaging strategy.
 *
 * @author Tim Rohaly.    <http://drupal.org/user/202830>
 */

// This is similar to how the Rules UI forms work, and may include a rules
// configuration form.

// Each shipping method builds its own PackageContext to store method-specific
// settings:
//   Set max weight per-method
//   Set strategy per-method
//   Set multiple origing/destination per-method

// Detect and use pluging
// Add packaging fields to product types - pkg_qty, dimensions, weight, box type
// Admin for box sizes - need some way for strategies to add admin
//   settings to the packaging admin menu.
// Markups?

// Contributed packaging method at http://drupal.org/node/444442

// Integration with Rules?
// Integration with Views?

// We need a way to save packaging results, so that shipping/label
// generation can use the same set of packages determined here.

/**
 * Default Packaging API settings.
 *
 * Builds Form API array containing all elements needed to configure and select
 * packaging strategy.  This form may be included on the individual shipping
 * quotes configuration pages or may be linked to from a list of available
 * shipping quote methods.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 *
 * @see packaging_admin_settings_validate()
 * @ingroup forms
 */
function packaging_admin_settings($form, &$form_state) {

  $form['max_weight'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Maximum package weight'),
    '#description' => t('Enter the maximum allowed package weight.'),
  );

  $form['weight_units'] = array(
    '#type'        => 'select',
    '#title'       => t('Package weight units'),
    '#description' => t('Select the default weight units to use for packages.'),
    '#options'     => array(
      'lb' => t('Pounds'),
      'oz' => t('Ounces'),
      'kg' => t('Kilograms'),
      'g'  => t('Grams'),
    ),
  );

  $form['max_volume'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Maximum package volume'),
    '#description' => t('Enter the maximum allowed package volume.'),
  );

  $form['length_units'] = array(
    '#type'        => 'select',
    '#title'       => t('Package length units'),
    '#description' => t('Select the default length units to use for packages.'),
    '#options'     => array(
      'in' => t('Inches'),
      'ft' => t('Feet'),
      'cm' => t('Centimeters'),
      'mm' => t('Millimeters'),
    ),
  );


  $operations = packaging_get_strategies();
  $options = array();
  foreach ($operations as $id => $operation) {
    $options[$id] = $operation['title'];
  }

  // Form to select packaging strategy.
  $form['packaging_strategy'] = array(
    '#type'          => 'select',
    '#title'         => t('Packaging strategy'),
    //'#default_value' => variable_get('packaging_strategy', 1),
    '#options'       => $options,
    '#description'   => t('Select the packaging strategy that is most appropriate to the types of products you sell.'),
    '#ajax' => array(
      'wrapper' => 'packaging-strategy-description-fieldset-wrapper',
      'callback' => 'packaging_strategy_select_callback',
    ),
  );

  // This fieldset serves as a container for the a description of the selected
  // packaging strategy.
  $form['packaging_strategy_description'] = array(
    '#type' => 'fieldset',
    '#title' => 'Strategy description',
    // These provide the wrapper referred to in #ajax['wrapper'] above.
    '#prefix' => '<div id="packaging-strategy-description-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  // The 'question' form element is examined in the validation function
  // ajax_example_dynamic_sections_validate(), so we must initialize it here.
  // Making it a hidden element allows us to give it a value without displaying
  // anything to the user.

  if (!empty($form_state['values']['packaging_strategy'])) {
    $strategy = $form_state['values']['packaging_strategy'];
  }
  else {
    reset($operations);
    $strategy = reset(array_keys($operations));
  }

  if ($instance = packaging_get_instance($strategy)) {
    $description = $instance->getDescription(); 
  }
  else {
    $description = t('No description available');
  }

  $form['packaging_strategy_description']['description'] = array(
    '#type' => 'markup',
    '#markup' => check_markup($description),
  );

  // Register additional submit handler.
  $form['#submit'][] = 'packaging_admin_settings_submit';

  return system_settings_form($form);

}
/**
 * Callback for the select element.
 *
 * This just selects and returns the questions_fieldset.
 */
function packaging_strategy_select_callback($form, $form_state) {
  return $form['packaging_strategy_description'];
}


/**
 * Form submission handler for packaging_admin_settings() form.
 */
function packaging_admin_settings_submit($form, &$form_state) {
  $operation = $form_state['values']['packaging_strategy'];
  if ($instance = packaging_get_instance($operation)) {
    drupal_set_message("Invoked packageProducts()<pre>" . var_export($instance->packageProducts(array(new Product(), new Product())), TRUE) ."</pre>");
  }
}

/**
 * Menu callback.
 */
function print_info() {

  $operations = packaging_get_strategies();
  $options = array();
  foreach ($operations as $id => $operation) {
    $options[$id] = $operation['title'];
  }

  $form['strategy'] = array(
    '#type' => 'select',
    '#title' => t('Please choose Strategy'),
    '#options' => $options,
  );

  $form['operations'] = array(
    '#markup' => '<pre>' . var_export($operations, TRUE) . '</pre>',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Select strategy'),
  );

  return $form;
}

/**
 * Submit handler.
 */
function print_info_submit($form, &$form_state) {
  $operation = $form_state['values']['strategy'];
  if ($instance = packaging_get_instance($operation)) {
    drupal_set_message("Invoked packageProducts()<pre>" . var_export($instance->packageProducts(array(new Product(), new Product())), TRUE) ."</pre>");
  }
}
