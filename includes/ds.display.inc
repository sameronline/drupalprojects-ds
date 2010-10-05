<?php
// $Id$

/**
 * Extend 'Manage display' table with regions and fields.
 *
 * @param $form
 *   The form to add extra regions and fields.
 * @param $form_state
 *   The current form state.
 */
function ds_extend_manage_display_table(&$form, $form_state) {
  // TODO stop if nothing found.

  // Get the entity, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Get the fields.
  $fields = ds_get_fields($entity_type, $bundle, $view_mode, TRUE, FALSE);
  $field_values = ds_get_fields_configuration($entity_type, $bundle, $view_mode);

  $table = &$form['fields'];
  $form['#ds_fields'] = array();

  // Add regions.
  $table['#header'] = array(
    t('Field'),
    t('Weight'),
    t('Parent'),
    t('Region'),
    t('Label'),
    array('data' => t('Format'), 'colspan' => 3),
  );
  $table['#regions'] = array(
    'visible' => array('title' => t('Header'), 'message' => t('No field is displayed.')),
    'left' => array('title' => t('Left'), 'message' => t('No field is displayed.')),
    'middle' => array('title' => t('Middle'), 'message' => t('No field is displayed.')),
    'right' => array('title' => t('Right'), 'message' => t('No field is displayed.')),
    'footer' => array('title' => t('Footer'), 'message' => t('No field is displayed.')),
    'hidden' => array('title' => t('Hidden'), 'message' => t('No field is hidden.')),
  );

  $field_label_options = array(
    'above' => t('Above'),
    'inline' => t('Inline'),
    'hidden' => t('<Hidden>'),
  );


  $region = array(
    '#type' => 'select',
    '#options' => ds_regions(),
    '#default_value' => 'hidden',
  );

  // Update existing rows by changing rowHandler and adding regions.
  foreach (element_children($table) as $name) {
    $row = &$table[$name];
    $row['#js_settings'] = array('rowHandler' => 'ds');
    // Add region between parent_wrapper & label.

    dsm($row);
    $row['region'] = $region;
    dsm($row);
  }

  foreach ($fields as $key => $field) {
    $form['#ds_fields'][] = $key;

    $formatters = isset($field['properties']['formatters']) ? $field['properties']['formatters'] : array('default' => t('Default'));

    $table[$key] = array(
      //'#parents' => array($key),
      '#row_type' => 'field',
      '#js_settings' => array('rowHandler' => 'ds'),
      '#attributes' => array('class' => array('draggable', 'tabledrag-leaf')),
      '#region_callback' => 'field_ui_display_overview_row_region', // TODO
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
          '#required' => FALSE,
          '#options' => array(), // TODO $table['#parent_options'],
          '#attributes' => array('class' => array('field-parent')),
          '#parents' => array('fields', $key, 'parent'),
        ),
        'hidden_name' => array(
          '#type' => 'hidden',
          '#default_value' => $key,
          '#attributes' => array('class' => array('field-name')),
        ),
      ),
      'region' => array(
        '#type' => 'select',
        '#options' => ds_regions(),
        '#default_value' => 'hidden',
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

  $form['#attached']['js'][] = drupal_get_path('module', 'ds') . '/js/ds.js';

  // Add extra submit handler.
  $form['#submit'][] = 'ds_form_field_ui_display_overview_form_alter_submit';
}

/**
 * Save the region and field settings from the 'Manage display' screen.
 */
function ds_extend_manage_display_table_submit($form, &$form_state) {

  // Get the entity_type, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Delete previous configuration.
  db_delete('ds_settings')
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('view_mode', $view_mode)
    ->execute();

  // Create a record to insert into the display_suite table.
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

}

/**
 * Helper function to add region field to existing rows.
 */
function array_insert(&$array, $element, $position = NULL) {
  if (count($array) == 0) {
    $array[] = $element;
  }
  elseif (is_numeric($position) && $position < 0) {
    if((count($array)+position) < 0) {
      $array = array_insert($array,$element,0);
    }
    else {
      $array[count($array)+$position] = $element;
    }
  }
  elseif (is_numeric($position) && isset($array[$position])) {
    $part1 = array_slice($array,0,$position,true);
    $part2 = array_slice($array,$position,null,true);
    $array = array_merge($part1,array($position=>$element),$part2);
    foreach($array as $key=>$item) {
      if (is_null($item)) {
        unset($array[$key]);
      }
    }
  }
  elseif (is_null($position)) {
    $array[] = $element;
  }
  elseif (!isset($array[$position])) {
    $array[$position] = $element;
  }
  $array = array_merge($array);
  return $array;
}
