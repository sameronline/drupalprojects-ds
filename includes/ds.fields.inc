<?php
// $Id$

/**
 * @file
 * Administrative functions for managing custom fields for every entity.
 */

/**
 * Shows the overview page for custom fields.
 */
function ds_custom_fields() {
  return 'jow';
}

/**
 * Shared form for all all fields.
 */
function ds_shared_form($field) {

  $custom_fields = ds_get_custom_fields();
  if (isset($custom_view_modes[$field])) {
    $field = $custom_view_modes[$field];
  }
  else {
    $field = new stdClass;
    $field->label = '';
    $field->field = '';
    $field->entities = array();
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $field->label,
    '#description' => t('The human-readable label of the field. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 30,
    '#field_suffix' => ' <small id="edit-label-suffix">' . t('Machine name: @field', array('@field' => $field->field)) . '</small>',
  );

  if (empty($field->field)) {
    $js_settings = array(
      'type' => 'setting',
      'data' => array(
        'machineReadableValue' => array(
          'label' => array(
            'text' => t('Machine name'),
            'target' => 'field',
            'searchPattern' => '[^a-z_]+',
            'replaceToken' => '_',
          ),
        ),
      ),
    );
    $form['field'] = array(
      '#title' => t('Machine name'),
      '#type' => 'textfield',
      '#default_value' => $field->field,
      '#maxlength' => 32,
      '#required' => TRUE,
      '#description' => t('The machine-readable name of this field. This name must contain only lowercase letters and underscores. This name must be unique.'),
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'system') . '/system.js', $js_settings),
      ),
    );
  }
  else {
    $form['field'] = array(
      '#type' => 'value',
      '#value' => $field->field
    );
  }

  $entity_options = array();
  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity_info) {
    if ($entity_info['fieldable']) {
      $entity_options[$entity_type] = drupal_ucfirst(str_replace('_', ' ', $entity_type));
    }
  }
  $form['entities'] = array(
    '#title' => t('Entities'),
    '#description' => t('Select the entities for which this field will be made available.'),
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#options' => $entity_options,
    '#default_value' => $field->entities,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  $form['old_field'] = array(
    '#type' => 'value',
    '#value' => $field->field,
  );

  // Validate & submit are also the same.
  $form['#validate'][] = 'ds_shared_form_validate';
  $form['#submit'][] = 'ds_shared_form_submit';

  return $form;
}

/**
 * Shared field form validation.
 */
function ds_shared_form_validate($form, &$form_state) {
  $field = new stdClass;
  $field->field = $form_state['values']['field'];
  $field->label = $form_state['values']['label'];

  $entities = $form_state['values']['entities'];
  foreach ($entities as $key => $value) {
    if ($key !== $value) {
      unset($entities[$key]);
    }
  }
  $field->entities = serialize($entities);

  $old_field = $form_state['values']['old_field'];

  $custom_fields = ds_get_custom_fields();

  if (isset($custom_fields[$field->field]) && $field->field != $old_field) {
    form_set_error('type', t('The machine-readable name %field is already taken.', array('%field' => $field->field)));
  }
  if (!preg_match('!^[a-z_]+$!', $field->field)) {
    form_set_error('type', t('The machine-readable name must contain only lowercase letters and underscores.'));
  }

  $form_state['field'] = $field;
}

/**
 * Save any field.
 */
function ds_shared_form_submit($form, &$form_state) {
  $field = $form_state['field'];

  // Delete previous field configuration.
  db_delete('ds_fields')
    ->condition('field', $field->field)
    ->execute();

  // Save field en clear entity info cache and rebuild menu.
  // TODO check this
  drupal_write_record('ds_fields', $field);
  cache_clear_all();

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/fields';
  drupal_set_message(t('The field %field has been saved', array('%field' => $field->label)));
}

/**
 * Manage a custom field.
 */
function ds_edit_custom_field_form($form, &$form_state, $custom_field = '') {
  $form += ds_shared_form($custom_field);
  return $form;
}

/**
 * Manage a custom block.
 */
function ds_edit_block_field_form($form, &$form_state, $custom_block = '') {
  $form += ds_shared_form($custom_block);
  return $form;
}

/**
 * Confirmation custom field delete form
 */
function ds_delete_field_confirm($form, &$form_state, $field = '') {
  $fields = ds_get_custom_fields();
  if (isset($fields[$field])) {
    $field = $fields[$field];
    $form['#field'] = $field;
    return confirm_form($form,
      t('Are you sure you want to delete %field?', array('%field' => $field->label)),
      'admin/structure/ds/fields',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
  else {
    drupal_set_message(t('Unknown field'));
    drupal_goto('admin/structure/ds/fields');
  }
}

/**
 * Confirmed field delete submit callback.
 */
function ds_delete_field_confirm_submit($form, &$form_state) {

  $field = $form['#field'];

  // Remove view mode.
  db_delete('ds_fields')
    ->condition('fields', $field->field)
    ->execute();

  // Clear cache.
  // TODO check this.
  cache_clear_all();

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/fields';
  drupal_set_message(t('The field %field has been deleted', array('%field' => $field->label)));
}
