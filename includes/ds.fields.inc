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
  $output = '';

  $custom_fields = ds_get_custom_fields();
  if (!empty($custom_fields)) {

    $rows = array();
    foreach ($custom_fields as $field_key => $field_value) {
      $row = array();
      $row[] = check_plain($field_value->label);
      $row[] = ds_field_human_name($field_value->type);
      $row[] = $field_key;
      $row[] = ucwords(str_replace('_', ' ', implode(', ', $field_value->entities)));
      $operations = l(t('Edit'), 'admin/structure/ds/fields/manage/' . $field_key, array('alias' => TRUE));
      $operations .= ' - ' . l(t('Delete'), 'admin/structure/ds/fields/delete/' . $field_key, array('alias' => TRUE));
      $row[] = $operations;
      $rows[] = $row;
    }

    $table = array(
      'header' => array(
        'Label',
        'Type',
        'Machine name',
        'Entities',
        'Operations',
      ),
      'rows' => $rows,
    );

    $output = theme('table', $table);
  }
  else {
    $output = t('No custom fields have been defined.');
  }

  return $output;
}

/**
 * Return the human name of a field.
 *
 * @return $human_name
 *   The human name of a field.
 */
function ds_field_human_name($type) {

  switch ($type) {
    case DS_FIELD_TYPE_CODE:
      return t('Code field');
    case DS_FIELD_TYPE_BLOCK:
      return t('Block field');
  }

  // Fallback
  return t('Unknown');
}

/**
 * Manage a field. This will redirect to the exact form.
 */
function ds_custom_manage($field_key = '') {
  $fields = ds_get_custom_fields();
  if (isset($fields[$field_key])) {
    $field = $fields[$field_key];
    switch ($field->type) {
      case DS_FIELD_TYPE_CODE:
        $redirect = 'admin/structure/ds/fields/manage_custom/' . $field_key;
        break;

      case DS_FIELD_TYPE_BLOCK:
        $redirect = 'admin/structure/ds/fields/manage_block/' . $field_key;
        break;

      default:
        drupal_set_message(t('Field not found'));
        $redirect = 'admin/structure/ds/fields';
        break;
    }
  }

  drupal_goto($redirect);
}

/**
 * Shared form for all all fields.
 */
function ds_shared_form(&$form, $field) {

  $custom_fields = ds_get_custom_fields();
  if (isset($custom_view_modes[$field])) {
    $field = $custom_view_modes[$field];
  }
  else {
    $field = new stdClass;
    $field->label = '';
    $field->field = '';
    $field->entities = array();
    $field->properties = array();
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
    '#value' => t('Save'),
    '#weight' => 100,
  );

  $form['old_field'] = array(
    '#type' => 'value',
    '#value' => $field->field,
  );

  // Validate & submit are also the same.
  $form['#validate'][] = 'ds_shared_form_validate';
  $form['#submit'][] = 'ds_shared_form_submit';

  return $field;
}

/**
 * Shared field form validation.
 */
function ds_shared_form_validate($form, &$form_state) {
  $field = new stdClass;
  $field->properties = new stdClass;
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
  $field->properties = serialize($field->properties);

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
  $custom_field = ds_shared_form($form, $custom_field);

  $form['code'] = array(
    '#type' => 'text_format',
    '#title' => t('Field code'),
    '#description' => t('If you are able to insert PHP code, the $entity variable is available to use. Eg $entity->nid is the id of a node.'),
    '#default_value' => isset($custom_field->properties->code) ? $custom_field->code : '',
    '#format' => isset($custom_field->properties->format) ? $custom_field->properties->format : 1,
    '#base_type' => 'textarea',
    '#required' => TRUE,
  );

  $form['#validate'][] = 'ds_custom_field_form_validate';

  return $form;
}

/**
 * Custom field form validation.
 */
function ds_custom_field_form_validate($form, &$form_state) {
  $form_state['field']->type = DS_FIELD_TYPE_CODE;
  $form_state['field']->properties->code = $form_state['values']['code'];
}

/**
 * Manage a custom block.
 */
function ds_edit_block_field_form($form, &$form_state, $custom_block = '') {
  $custom_block = ds_shared_form($form, $custom_block);

  $blocks = array();
  foreach (module_implements('block_info') as $module) {
    $module_blocks = module_invoke($module, 'block_info');
    if ($module_blocks) {
      foreach ($module_blocks as $module_key => $info) {
        $blocks[ucfirst($module)][$module .'|'. $module_key] = $info['info'];
      }
    }
  }
  ksort($blocks);

  $form['block_identity']['block'] = array(
    '#type' => 'select',
    '#options' => $blocks,
    '#title' => t('Block'),
    '#required' => TRUE,
    '#default_value' => isset($field->properties->block) ? $field->properties->block : '',
  );
  $form['block_identity']['block_render'] = array(
    '#type' => 'select',
    '#options' => array(
      DS_BLOCK_TEMPLATE => t('Render through block template'),
      DS_BLOCK_TITLE_CONTENT => t('Show block title + content'),
      DS_BLOCK_CONTENT => t('Show only block content'),
    ),
    '#title' => t('Block render'),
    '#required' => TRUE,
    '#default_value' => isset($field->properties->block_render) ? $field->properties->block_render : '',
  );

  $form['#validate'][] = 'ds_block_field_form_validate';

  return $form;
}

/**
 * Custom field form validation.
 */
function ds_block_field_form_validate($form, &$form_state) {
  $form_state['field']->type = DS_FIELD_TYPE_BLOCK;
  $form_state['field']->properties->block = $form_state['values']['block'];
  $form_state['field']->properties->block_render = $form_state['values']['block_render'];
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
    ->condition('field', $field->field)
    ->execute();

  // Clear cache.
  // TODO check this.
  cache_clear_all();

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/fields';
  drupal_set_message(t('The field %field has been deleted', array('%field' => $field->label)));
}
