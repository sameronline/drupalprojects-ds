<?php
// $Id$

/**
 * @file
 * Field UI form alters for Display Suite.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function ds_form_field_ui_field_overview_form_alter(&$form, &$form_state) {
  ds_extend_field_ui_table($form, $form_state);
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function ds_form_field_ui_display_overview_form_alter(&$form, &$form_state) {
  ds_extend_field_ui_table($form, $form_state);
}

/**
 * Extend 'Manage fields/display' table with layout and fields.
 *
 * @param $form
 *   The form to add layout fieldset and extra display suite fields.
 * @param $form_state
 *   The current form state.
 */
function ds_extend_field_ui_table(&$form, $form_state) {

  // Get the entity_type, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = isset($form['#view_mode']) ? $form['#view_mode'] : 'form';

  // Alter the text of the custom display settings.
  if ($form['form_id']['#value'] == 'field_ui_display_overview_form') {
    $form['modes']['view_modes_custom']['#description'] = t('<a href="!url">Create new view modes</a>', array('!url' => url('admin/structure/ds/view_modes')));
  }

  // Add extra fields.
  ds_extend_field_ui_table_fields($entity_type, $bundle, $view_mode, $form);

  // Add layouts fieldset.
  ds_extend_field_ui_table_layouts($entity_type, $bundle, $view_mode, $form);

  // Add extra submit handler.
  $form['#submit'][] = 'ds_extend_field_ui_table_submit';
}

/**
 * Adds the Display Suite fields to the form.
 *
 * @param $entity_type
 *   The name of the entity type.
 * @param $bundle
 *   The name of the bundle
 * @param $view_mode
 *   The name of the view_mode
 * @param $form
 *   A collection of form properties.
 */
function ds_extend_field_ui_table_fields($entity_type, $bundle, $view_mode, &$form) {

  // Get the fields.
  $fields = ds_get_fields($entity_type, $bundle, $view_mode, TRUE, FALSE);
  $field_values = ds_get_fields_configuration($entity_type, $bundle, $view_mode);

  $table = &$form['fields'];
  $form['#ds_fields'] = array();

  $field_label_options = array(
    'above' => t('Above'),
    'inline' => t('Inline'),
    'hidden' => t('<Hidden>'),
  );

  foreach ($fields as $key => $field) {
    $form['#ds_fields'][] = $key;

    $formatters = array('hidden' => t('<Hidden>'));
    $formatters += isset($field['properties']['formatters']) ? $field['properties']['formatters'] : array('default' => t('Default'));

    $table[$key] = array(
      //'#parents' => array($key),
      '#row_type' => 'field',
      '#js_settings' => array('rowHandler' => 'field'),
      '#attributes' => array('class' => array('draggable', 'tabledrag-leaf')),
      '#region_callback' => 'field_ui_display_overview_row_region',
      'human_name' => array(
        '#markup' => check_plain($field['title']),
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => isset($field_values[$key]['weight']) ? $field_values[$key]['weight'] : 0,
        '#size' => 3,
        '#attributes' => array('class' => array('field-weight')),
      ),
      'parent_wrapper' => array(
        'parent' => array(
          '#type' => 'select',
          '#empty_value' => '',
          '#options' => array(), // This will be filled by field_group. Default value as well.
          '#attributes' => array('class' => array('field-parent')),
          '#parents' => array('fields', $key, 'parent'),
        ),
        'hidden_name' => array(
          '#type' => 'hidden',
          '#default_value' => $key,
          '#attributes' => array('class' => array('field-name')),
        ),
      ),
      'label' => array(
        '#type' => 'select',
        '#options' => $field_label_options,
        '#default_value' => isset($field_values[$key]['label']) ? $field_values[$key]['label'] : 'hidden',
      ),
      'format' => array(
        'type' => array(
          '#type' => 'select',
          '#options' => $formatters,
          '#default_value' => isset($field_values[$key]['format']) ? $field_values[$key]['format'] : 'hidden',
          '#attributes' => array('class' => array('field-formatter-type')),
        ),
      ),
      'settings_summary' => array(),
      'settings_edit' => array(),
    );
  }
}

/**
 * Adds the layouts fieldset to the form.
 *
 * @param $entity_type
 *   The name of the entity type.
 * @param $bundle
 *   The name of the bundle
 * @param $view_mode
 *   The name of the view_mode
 * @param $form
 *   A collection of form properties.
 */
function ds_extend_field_ui_table_layouts($entity_type, $bundle, $view_mode, &$form) {

  $layout_options = array();
  $ds_layouts = module_invoke_all('ds_layouts');
  $layout_options = array('' => t(' - None - '));
  foreach ($ds_layouts as $key => $layout) {
    $layout_options[$key] = $layout['label'];
  }

  // Add layouts form.
  $layout = db_query('SELECT layout, load_css, load_template, load_html5 FROM {ds_layouts} WHERE entity_type = :entity_type AND bundle = :bundle AND view_mode = :view_mode', array(':entity_type' => $entity_type, ':bundle' => $bundle, ':view_mode' => $view_mode))->fetchObject();
  $form['ds_layouts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layouts'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (empty($layout)) {
    $form['ds_layouts']['layout'] = array(
      '#type' => 'select',
      '#title' => t('Select a layout'),
      '#options' => $layout_options,
      '#description' => t('Select a layout for !bundle in !view_mode.', array('!bundle' => $bundle, '!view_mode' => $view_mode)),
    );
  }
  else {
    $form['ds_layouts']['layout'] = array('#type' => 'value', '#value' => $layout->layout);
    $form['ds_layouts']['existing'] = array('#type' => 'value', '#value' => TRUE);
    $form['ds_layouts']['current'] = array('#markup' => '<p>'. t('You have chosen the %layout layout.', array('%layout' => $ds_layouts[$layout->layout]['label'])) .'</p>');
  }

  // Load css & template file.
  $form['ds_layouts']['load_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load stylesheet'),
    '#default_value' => isset($layout->load_css) ? $layout->load_css : 1,
  );

  $form['ds_layouts']['load_template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load template file'),
    '#default_value' => isset($layout->load_template) ? $layout->load_template : 1,
  );

  $form['ds_layouts']['load_html5'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load the HTML 5 version of the template file'),
    '#default_value' => isset($layout->load_html5) ? $layout->load_html5 : 0,
  );
}

/**
 * Save the layout and field settings from the 'Manage display' screen.
 */
function ds_extend_field_ui_table_save($form, &$form_state) {
  // Get the entity_type, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = isset($form['#view_mode']) ? $form['#view_mode'] : 'form';
  $layout_new = isset($form_state['values']['existing']) ? FALSE : TRUE;

  // Delete previous field configuration configuration.
  db_delete('ds_settings')
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('view_mode', $view_mode)
    ->execute();

  // Create a record to insert into the ds_settings table.
  $record = new stdClass;
  $record->entity_type = $entity_type;
  $record->bundle = $bundle;
  $record->view_mode = $view_mode;

  // Save settings for each field.
  $fields = $form['#ds_fields'];
  foreach ($fields as $key => $field) {

    // Field values.
    $field_values = $form_state['values']['fields'][$field];

    // Build settings.
    $settings = array();
    $settings['weight'] = $field_values['weight'];
    $settings['label'] = $field_values['label'];
    $settings['format'] = $field_values['format']['type'];

    // Save the record.
    $record->field = $field;
    $record->settings = serialize($settings);
    drupal_write_record('ds_settings', $record);
  }

  // Clear the ds_fields_configuration cache.
  cache_clear_all('ds_fields_configuration', 'cache');

  // Save layout and add regions if necessary.
  $record = new stdClass;
  $record->entity_type = $entity_type;
  $record->bundle = $bundle;
  $record->view_mode = $view_mode;
  $record->layout = $form_state['values']['layout'];
  $record->load_css = $form_state['values']['load_css'];
  $record->load_template = $form_state['values']['load_template'];
  $record->load_html5 = $form_state['values']['load_html5'];

  if ($layout_new) {
    // Save layout.
    drupal_write_record('ds_layouts', $record);

    // Group records.
    $weight = -100;
    $ds_layouts = module_invoke_all('ds_layouts');
    $layout = $ds_layouts[$record->layout];
    foreach ($layout['regions'] as $region => $label) {
      $record = new stdClass;
      $record->group_name = 'group_' . $region;
      $record->entity_type = $entity_type;
      $record->bundle = $bundle;
      $record->mode = $view_mode;
      $record->data = array(
        'label' => $label,
        'weight' => $weight++,
        'children' => array(),
        'visible' => 'region',
      );
      drupal_write_record('field_group', $record);
    }
  }
  else {
    // Update existing layout.
    drupal_write_record('ds_layouts', $record, array('entity_type', 'bundle', 'view_mode', 'layout'));
  }

  // Clear entity cache.
  cache_clear_all('entity_info', 'cache', TRUE);
}
