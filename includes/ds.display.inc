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
  $form['fields']['#regions'] = array(
    'visible' => array('title' => t('Header'), 'message' => t('No field is displayed.')),
    'footer' => array('title' => t('Footer'), 'message' => t('No field is displayed.')),
    'hidden' => array('title' => t('Hidden'), 'message' => t('No field is hidden.')),
  );

  $field_label_options = array(
    'above' => t('Above'),
    'inline' => t('Inline'),
    'hidden' => t('<Hidden>'),
  );

  // Update existing rows accordingly.
  foreach (element_children($table) as $name) {
    $row = &$table[$name];
    $row['#js_settings'] = array('rowHandler' => 'ds');
  }

  foreach ($fields as $key => $field) {
    $form['#ds_fields'][] = $key;

    $formatters = isset($field['properties']['formatters']) ? $field['properties']['formatters'] : array('default' => t('Default'));
    $formatters['hidden'] = t('<Hidden>');

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
