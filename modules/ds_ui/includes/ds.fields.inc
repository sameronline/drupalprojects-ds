<?php

/**
 * @file
 * Administrative functions for managing custom fields for every entity.
 */

/**
 * Shows the list of custom fields.
 */
function ds_custom_fields_list() {
  $output = '';

  $custom_fields = config_get_storage_names_with_prefix('ds.field.');
  if (!empty($custom_fields)) {

    $rows = array();
    foreach ($custom_fields as $config) {
      $field_value = config($config)->get();
      $row = array();
      $row[] = check_plain($field_value['label']);
      $row[] = ds_field_human_name($field_value['field_type']);
      $row[] = $field_value['field'];
      $row[] = ucwords(str_replace('_', ' ', implode(', ', $field_value['entities'])));

      $operations = array();
      $operations['edit'] = array(
        'title' => t('Edit'),
        'href' => 'admin/structure/ds/fields/manage/' . $field_value['field'],
      );
      $operations['delete'] = array(
        'title' => t('Delete'),
        'href' => 'admin/structure/ds/fields/delete/' . $field_value['field'],
      );
      $row[] = array(
        'data' => array(
          '#type' => 'operations',
          '#subtype' => 'ds',
          '#links' => $operations,
        ),
      );

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
    case DS_FIELD_TYPE_PREPROCESS:
      return t('Preprocess field');
  }

  // Fallback
  return t('Unknown');
}

/**
 * Manage a field. This will redirect to the exact form.
 */
function ds_custom_manage($field_key = '') {
  $redirect = '';
  if ($field = config('ds.field.' . $field_key)->get()) {
    $field = $custom_fields[$field_key];
    switch ($field['field_type']) {
      case DS_FIELD_TYPE_CODE:
        $redirect = 'admin/structure/ds/fields/manage_custom/' . $field_key;
        break;

      case DS_FIELD_TYPE_BLOCK:
        $redirect = 'admin/structure/ds/fields/manage_block/' . $field_key;
        break;

      case DS_FIELD_TYPE_PREPROCESS:
        $redirect = 'admin/structure/ds/fields/manage_preprocess/' . $field_key;
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

  if (!empty($field)) {
    $field = config('ds.field.' . $field)->get();
  }

  if (!$field) {
    $field = array();
    $field['label'] = '';
    $field['field'] = '';
    $field['ui_limit'] = '';
    $field['entities'] = array();
    $field['properties'] = array();
  }

  $form['name'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $field['label'],
    '#description' => t('The human-readable label of the field.'),
    '#maxlength' => 32,
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['field'] = array(
    '#type' => 'machine_name',
    '#default_value' => $field['field'],
    '#maxlength' => 32,
    '#description' => t('The machine-readable name of this field. This name must contain only lowercase letters and underscores. This name must be unique.'),
    '#disabled' => !empty($field['field']),
    '#machine_name' => array(
      'exists' => 'ds_field_unique',
    ),
  );

  $entity_options = array();
  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity_info) {
    if ((isset($entity_info['fieldable']) && $entity_info['fieldable']) || $entity_type == 'ds_views') {
      $entity_options[$entity_type] = drupal_ucfirst(str_replace('_', ' ', $entity_type));
    }
  }
  $form['entities'] = array(
    '#title' => t('Entities'),
    '#description' => t('Select the entities for which this field will be made available.'),
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#options' => $entity_options,
    '#default_value' => $field['entities'],
  );

  $form['ui_limit'] = array(
    '#title' => t('Limit field'),
    '#description' => t('Limit this field on field UI per bundles and/or view modes. The values are in the form of $bundle|$view_mode. You may use * to select all. Enter multiple values per line.'),
    '#type' => 'textarea',
    '#default_value' => $field['ui_limit'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  // Validate & submit are also the same.
  $form['#validate'][] = 'ds_shared_form_validate';
  $form['#submit'][] = 'ds_shared_form_submit';

  return $field;
}

/**
 * Return whether a field machine name is unique.
 */
function ds_field_unique($name) {
  $value = strtr($name, array('-' => '_'));
  if (config('ds.field.' . $value)->get()) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Shared field form validation.
 */
function ds_shared_form_validate($form, &$form_state) {
  $field = array();
  $field['properties'] = array();
  $field['field'] = $form_state['values']['field'];
  $field['label'] = $form_state['values']['name'];
  $field['ui_limit'] = $form_state['values']['ui_limit'];

  $entities = $form_state['values']['entities'];
  foreach ($entities as $key => $value) {
    if ($key !== $value) {
      unset($entities[$key]);
    }
  }
  $field['entities'] = $entities;
  $form_state['field'] = $field;
}

/**
 * Save any field.
 */
function ds_shared_form_submit($form, &$form_state) {
  $field = $form_state['field'];

  // Save field and clear ds_fields.
  config('ds.field.' . $field['field'])->setData($field)->save();
  cache()->Prefix('ds_fields');

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/fields';
  drupal_set_message(t('The field %field has been saved.', array('%field' => $field['label'])));
}

/**
 * Manage a custom field.
 */
function ds_edit_custom_field_form($form, &$form_state, $custom_field = '') {
  $custom_field = ds_shared_form($form, $custom_field);

  $form['code'] = array(
    '#type' => 'text_format',
    '#title' => t('Field code'),
    '#default_value' => isset($custom_field['properties']['code']['value']) ? $custom_field['properties']['code']['value'] : '',
    '#format' => isset($custom_field['properties']['code']['format']) ? $custom_field['properties']['code']['format'] : 'ds_code',
    '#base_type' => 'textarea',
    '#required' => TRUE,
  );

  $form['use_token'] = array(
    '#type' => 'checkbox',
    '#title' => t('Token'),
    '#description' => t('Toggle this checkbox if you are using tokens in this field.'),
    '#default_value' => isset($custom_field['properties']['use_token']) ? $custom_field['properties']['use_token'] : '',
  );

  // Token support.
  if (module_exists('token')) {

    $form['tokens'] = array(
      '#title' => t('Tokens'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => array(
        'invisible' => array(
          'input[name="use_token"]' => array('checked' => FALSE),
        ),
      ),
    );
    $form['tokens']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => 'all',
      '#global_types' => FALSE,
    );
  }
  else {
    $form['use_token']['#description'] = t('Toggle this checkbox if you are using tokens in this field. If the token module is installed, you get a nice list of all tokens available in your site.');
  }

  $form['#validate'][] = 'ds_custom_field_form_validate';

  return $form;
}

/**
 * Custom field form validation.
 */
function ds_custom_field_form_validate($form, &$form_state) {
  $form_state['field']['field_type'] = DS_FIELD_TYPE_CODE;
  $form_state['field']['properties']['code'] = $form_state['values']['code'];
  $form_state['field']['properties']['use_token'] = $form_state['values']['use_token'];
}

/**
 * Manage a Preprocess field.
 */
function ds_edit_preprocess_field_form($form, &$form_state, $preprocess_field = '') {
  $custom_field = ds_shared_form($form, $preprocess_field);
  $form['info'] = array(
    '#markup' => t('The machine name of this field must reflect the key in the variables, e.g. "submitted". So in most cases, it is very likely you will have to manually edit the machine name as well, which can not be changed anymore after saving. Note that this field type works best on Nodes.'),
    '#weight' => -10,
  );
  $form['#validate'][] = 'ds_preprocess_field_form_validate';
  return $form;
}

/**
 * Preprocess field form validation.
 */
function ds_preprocess_field_form_validate($form, &$form_state) {
  $form_state['field']['field_type'] = DS_FIELD_TYPE_PREPROCESS;
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
        $blocks[drupal_ucfirst($module)][$module . '|' . $module_key] = $info['info'];
      }
    }
  }
  ksort($blocks);

  $form['block_identity']['block'] = array(
    '#type' => 'select',
    '#options' => $blocks,
    '#title' => t('Block'),
    '#required' => TRUE,
    '#default_value' => isset($custom_block['properties']['block']) ? $custom_block['properties']['block'] : '',
  );
  $form['block_identity']['block_render'] = array(
    '#type' => 'select',
    '#options' => array(
      DS_BLOCK_TEMPLATE => t('Default'),
      DS_BLOCK_TITLE_CONTENT => t('Show block title + content'),
      DS_BLOCK_CONTENT => t('Show only block content'),
    ),
    '#title' => t('Layout'),
    '#required' => TRUE,
    '#default_value' => isset($custom_block['properties']['block_render']) ? $custom_block['properties']['block_render'] : '',
  );

  $form['#validate'][] = 'ds_block_field_form_validate';

  return $form;
}

/**
 * Custom field form validation.
 */
function ds_block_field_form_validate($form, &$form_state) {
  $form_state['field']['field_type'] = DS_FIELD_TYPE_BLOCK;
  $form_state['field']['properties'] = array();
  $form_state['field']['properties']['block'] = $form_state['values']['block'];
  $form_state['field']['properties']['block_render'] = $form_state['values']['block_render'];
}

/**
 * Menu callback: Confirmation custom field delete form.
 */
function ds_delete_field_confirm($form, &$form_state, $field = '') {
  return ds_remove_fields_form($form, $form_state, $field);
}

/**
 * Confirmation delete or revert form.
 */
function ds_remove_fields_form($form, &$form_state, $field = '') {
  if ($field = config('ds.field.' . $field)->get()) {
    $form['#ds_field'] = $field;
    return confirm_form($form,
      t('Are you sure you want to delete %field ?', array('%field' => $field['label'])),
      'admin/structure/ds/fields',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
  else {
    drupal_set_message(t('Unknown field.'));
    drupal_goto('admin/structure/ds/fields');
  }
}

/**
 * Submit callback: confirmed delete submit.
 */
function ds_delete_field_confirm_submit($form, &$form_state) {
  ds_remove_field_confirm_submit($form, $form_state, 'deleted');
}

/**
 * Confirmed field delete submit callback.
 */
function ds_remove_field_confirm_submit($form, &$form_state) {

  $field = $form['#ds_field'];

  // Remove field.
  config('ds.field.' . $field['field'])->delete();

  // Clear cache.
  cache()->deletePrefix('ds_fields');

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/fields';
  drupal_set_message(t('The field %field has been deleted', array('%field' => $field['label'])));
}

