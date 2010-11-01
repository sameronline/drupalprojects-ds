<?php
// $Id$

/**
 * Extend 'Manage display' table with layout and fields.
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

    $formatters = isset($field['properties']['formatters']) ? $field['properties']['formatters'] : array('default' => t('Default'));

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
  $layout = db_query('SELECT layout FROM {ds_layouts} WHERE entity_type = :entity_type AND bundle = :bundle AND view_mode = :view_mode', array(':entity_type' => $entity_type, ':bundle' => $bundle, ':view_mode' => $view_mode))->fetchField();
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
    $layout = $ds_layouts[$layout];
    $form['ds_layouts']['current'] = array('#markup' => t('You have chosen the %layout layout.', array('%layout' => $layout['label'])));
  }
}

/**
 * Save the layout and field settings from the 'Manage display' screen.
 */
function ds_extend_field_ui_table_save($form, &$form_state) {
  // Get the entity_type, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = isset($form['#view_mode']) ? $form['#view_mode'] : 'form';

  // Save layout and add regions if necessary.
  $layout = isset($form_state['values']['layout']) ? $form_state['values']['layout'] : 0;
  if (!empty($layout)) {

    // Layout record.
    $record = new stdClass;
    $record->entity_type = $entity_type;
    $record->bundle = $bundle;
    $record->view_mode = $view_mode;
    $record->layout = $layout;
    drupal_write_record('ds_layouts', $record);

    // Group records.
    $weight = -100;
    $ds_layouts = module_invoke_all('ds_layouts');
    $layout = $ds_layouts[$layout];
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

    // Clear entity cache.
    cache_clear_all('entity_info', 'cache', TRUE);
  }

}
