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
  form_load_include($form_state, 'inc', 'field_ui', 'field_ui.admin');
  $build = drupal_get_form('field_ui_display_overview_form', 'ds_views', 'views', 'default');

  // Deny access to view mode.
  $build['modes']['#access'] = FALSE;

  return $build;
}