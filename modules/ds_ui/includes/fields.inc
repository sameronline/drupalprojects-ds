<?php

/**
 * @file
 * Administrative functions for managing custom fields for every entity.
 */

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
  cache()->delete('ds_fields');

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/fields';
  drupal_set_message(t('The field %field has been deleted', array('%field' => $field['label'])));
}

