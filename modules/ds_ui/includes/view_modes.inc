<?php

/**
 * @file
 * Administrative functions for managing view modes for every entity.
 */

/**
 * Menu callback: Confirmation view mode delete form.
 */
function ds_delete_view_mode_confirm($form, &$form_state, $view_mode = '') {
  return ds_remove_view_mode_confirm($form, $form_state, $view_mode);
}

/**
 * Confirmation revert or remove form.
 */
function ds_remove_view_mode_confirm($form, &$form_state, $view_mode = '') {

  if ($view_mode = config('ds.view_modes.' . $view_mode)->get()) {
    $form['#view_mode'] = $view_mode;
    return confirm_form($form,
      t('Are you sure you want to delete %view_mode ?', array('%view_mode' => $view_mode['label'])),
      'admin/structure/ds/view_modes',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
  else {
    drupal_set_message(t('View mode not found.'));
    drupal_goto('admin/structure/ds/view_modes');
  }
}

/**
 * Confirmed view mode delete submit callback.
 */
function ds_delete_view_mode_confirm_submit($form, &$form_state) {
  ds_remove_view_mode($form, $form_state, 'deleted');
}

/**
 * Remove a view mode from the database.
 *
 * @param $action
 *   Whether we delete or remove the view mode.
 */
function ds_remove_view_mode($form, &$form_state) {

  $view_mode = $form['#view_mode'];

  // Remove view mode from database.
  config('ds.view_modes.' . $view_mode['view_mode'])->delete();

  // Remove layout and field settings for this view mode.
  $field_settings = config_get_storage_names_with_prefix('ds.field_settings');
  foreach ($field_settings as $config) {
    if (config($config)->get('view_mode') == $view_mode['view_mode']) {
      config($config)->delete();
    }
  }

  $layout_settings = config_get_storage_names_with_prefix('ds.layout_settings');
  foreach ($layout_settings as $config) {
    if (config($config)->get('view_mode') == $view_mode['view_mode']) {
      config($config)->delete();
    }
  }

  // Clear entity info cache and trigger menu build on next request.
  entity_info_cache_clear();
  variable_set('menu_router_rebuild_needed', TRUE);

  // Redirect.
  $form_state['redirect'] = 'admin/structure/ds/view_modes';
  drupal_set_message(t('The view mode %view_mode has been deleted.', array('%view_mode' => $view_mode['label'])));
}
