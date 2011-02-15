<?php
// $Id$

/**
 * @file
 * Views displays functions.
 */

/**
 * Manage the display of views.
 */
function ds_extras_vd($view = '', $bundle = '') {

  $entity_info = entity_get_info('ds_views');
  //dsm($entity_info);

  if (empty($view) && empty($bundle)) {
    return ds_extras_vd_overview($entity_info);
  }
  elseif (!empty($view) && !empty($bundle)) {
    return ds_extras_vd_field_ui($view, $bundle);
  }
}

/**
 * Show the overview form and the selection list.
 */
function ds_extras_vd_overview($entity_info) {
  return 'Views list';
}

/**
 * Return Field UI display screen for a view and bundle.
 */
function ds_extras_vd_field_ui($view, $bundle) {
  $form_state = array();
  $arguments = array('ds_views', 'views', 'default');
  $form_state['build_info']['args'] = $arguments;
  form_load_include($form_state, 'inc', 'field_ui', 'field_ui.admin');
  $build = drupal_build_form('field_ui_display_overview_form', $form_state);

  // Get rid of the notice.

  // Deny access to view mode.
  $build['additional_settings']['modes']['#access'] = FALSE;

  return $build;
}
