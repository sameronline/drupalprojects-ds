<?php
// $Id$

/**
 * @file
 * Registry file for Display Suite.
 */

/**
 * Menu implementations.
 */
function _ds_menu() {
  $items = array();

  $items['admin/structure/ds'] = array(
    'title' => 'Display suite',
    'description' => 'Manage configuration for Display Suite.',
    'page callback' => 'ds_configuration',
    'access arguments' => array('administer ds'),
  );

  // Custom view modes.
  $items['admin/structure/ds/view_modes'] = array(
    'title' => 'View modes',
    'description' => 'Manage custom view modes for all entities.',
    'page callback' => 'ds_custom_view_modes',
    'file' => 'includes/ds.view_modes.inc',
    'access arguments' => array('admin_view_modes'),
  );
  $items['admin/structure/ds/view_modes/manage'] = array(
    'title' => 'Add a view mode',
    'description' => 'Manage a view mode',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_edit_view_mode_form'),
    'file' => 'includes/ds.view_modes.inc',
    'access arguments' => array('admin_view_modes'),
    'type' => MENU_LOCAL_ACTION,
  );
  $items['admin/structure/ds/view_modes/delete'] = array(
    'title' => 'Delete a view mode',
    'description' => 'Delete a custom view mode.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_delete_view_mode_confirm'),
    'file' => 'includes/ds.view_modes.inc',
    'access arguments' => array('admin_view_modes'),
    'type' => MENU_CALLBACK,
  );




  return $items;
}

/**
 * Theme implementations.
 */
function _ds_theme() {

  // Display Suite theme functions.
  $theme_functions = array(
    // Evaluate code.
    'ds_eval_code' => array(
      'variables' => array('field' => NULL),
    ),
    // Evaluate block.
    'ds_eval_block' => array(
      'variables' => array('field' => NULL),
    ),
  );

  // Load theming functions from entities we implement.
  // TODO entity registration.
  ds_load_entity_implementations();
  foreach (array('node') as $entity_type) {
    module_load_include('inc', 'ds', 'entities/' . $entity_type . '/' . $entity_type);
    $enitity_type_theme_function = $entity_type . '_ds_theme_registry';
    $theme_functions += $enitity_type_theme_function();
  }

  return $theme_functions;
}

/**
 * Entity info alter.
 */
function _ds_entity_info_alter(&$entity_info) {

  // Add custom view modes to entities.
  $custom_view_modes = ds_get_view_modes();
  foreach ($custom_view_modes as $view_mode_key => $view_mode_value) {
    $view_mode = array(
      'label' => check_plain($view_mode_value->label),
      'custom settings' => TRUE,
    );
    foreach ($view_mode_value->entities as $entity_type) {
      if (isset($entity_info[$entity_type])) {
        $entity_info[$entity_type]['view modes'][$view_mode_key] = $view_mode;
      }
    }
  }
}
