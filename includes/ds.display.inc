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

  $layout_options = array();
  $ds_layouts = module_invoke_all('ds_layouts');
  $layout_options = array('' => t('<None>'));
  foreach ($ds_layouts as $key => $layout) {
    $layout_options[$key] = $layout['label'];
  }

  // @todo needs more work - like what with reselect and stuff - or might need a delete button to reset the layout ?

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
    $form['ds_layouts']['current'] = array('#markup' => 'Current layout chosen: '. $layout .' - remove button is coming later :)');
  }

  // Add extra submit handler.
  $form['#submit'][] = 'ds_extend_field_ui_table_submit';
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
  }

}
