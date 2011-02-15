<?php
// $Id$

/**
 * @file
 * Views displays functions.
 */

/**
 * Manage the display of views.
 */
function ds_extras_vd($view = '') {
  module_load_include('inc', 'field_ui', 'field_ui.admin');
  $form = array();
  $form_state = array();
  $entity_type = 'ds_views';
  $bundle = 'ds_vd_default';
  $form_id = 'field_ui_display_overview_form';
  $build = drupal_get_form($form_id, $entity_type, $bundle, 'default');
  return $build;
}