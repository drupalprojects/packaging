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

  // Use CTools to look for any modules which define packaging strategies.
  $operations = packaging_get_strategies();
  $options = array();
  foreach ($operations as $id => $operation) {
    $options[$id] = $operation['title'];
  }

  // Form to select packaging strategy.
  $form['packaging_strategy'] = array(
    '#type'          => 'select',
    '#title'         => t('Packaging strategy'),
    '#description'   => t('Select the packaging strategy that is most appropriate to the types of products you sell.'),
    '#element_validate' => array('packaging_strategy_element_validate'),
    '#options'       => $options,
    '#default_value' => variable_get('packaging_strategy', reset($options)),
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


  if (!empty($form_state['values']['packaging_strategy'])) {
    $strategy = $form_state['values']['packaging_strategy'];
  }
  else {
    $strategy = variable_get('packaging_strategy', reset($options));
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

  // Sets maximum allowed package weight.
  $form['packaging_max_weight'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Maximum package weight'),
    '#description' => t('Enter the maximum allowed package weight.'),
  );

  // Defines units for maximum weight, also used as the default weight units
  // for PackagingPackage objects.
  $form['packaging_weight_units'] = array(
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

  // Sets maximum allowed package volume.
  $form['packaging_max_volume'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Maximum package volume'),
    '#description' => t('Enter the maximum allowed package volume.'),
  );

  // Defines units for length, cubed for maximum volume. Also used as the
  // default length units for PackagingPackage objects.
  $form['packaging_length_units'] = array(
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
 * Element validation handler for packaging_admin_settings() form.
 */
function packaging_strategy_element_validate($element, &$form_state) {
  $operation = $element['#value'];
  $instance = packaging_get_instance($operation);
  if (!isset($instance)) {
    form_error($element, t('The %operation strategy could not be located. Please contact the site administrator.', array('%operation' => $operation)));
  }
}

/**
 * Form submission handler for packaging_admin_settings() form.
 */
function packaging_admin_settings_submit($form, &$form_state) {
  // Print message for testing purposes - won't do this in release version.
  $operation = $form_state['values']['packaging_strategy'];
  if ($instance = packaging_get_instance($operation)) {
    $context = new PackagingContext();
    $context->setStrategy($instance);
    drupal_set_message("Invoked packageProducts()<pre>" . var_export($context->packageProducts(array(new PackagingProduct(), new PackagingProduct())), TRUE) . "</pre>");
  }
}

/**
 * Menu callback for /packaging path. This is used only for testing during
 * development, and will be removed before a release.
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
    '#default_value' => variable_get('strategy', reset($options)),
  );

  //$form['hooks'] = array(
  //  '#markup' => '<pre>' .  ctools_plugin_api_get_hook('views', 'views') . '</pre>',
  //);

  $form['operations'] = array(
    '#markup' => '<pre>' . var_export($operations, TRUE) . '</pre>',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Select strategy'),
  );

  // Register additional submit handler.
  $form['#submit'][] = 'print_info_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for /packaging path. This is used only for testing during
 * development, and will be removed before a release.
 */
function print_info_submit($form, &$form_state) {
  $operation = $form_state['values']['strategy'];
  if ($instance = packaging_get_instance($operation)) {
    $context = new PackagingContext();
    $context->setStrategy($instance);
    drupal_set_message("Invoked packageProducts()<pre>" . var_export($context->packageProducts(array(new PackagingProduct(), new PackagingProduct())), TRUE) . "</pre>");
  }
}
