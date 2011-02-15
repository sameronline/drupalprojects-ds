<?php
// $Id$

/**
 * @file
 * Views displays functions.
 */

/**
 * Manage the display of views.
 */
function ds_extras_vd($view = '', $layout = '') {
  $form_state = array();
  $arguments = array('ds_views', 'views', 'default');
  $form_state['build_info']['args'] = $arguments;
  form_load_include($form_state, 'inc', 'field_ui', 'field_ui.admin');
  $build = drupal_build_form('field_ui_display_overview_form', $form_state);

  // Deny access to view mode.
  $build['additional_settings']['modes']['#access'] = FALSE;

  return $build;
}