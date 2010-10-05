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

  // Start page of Display Suite.
  $items['admin/structure/ds'] = array(
    'title' => 'Display suite',
    'description' => 'Manage configuration for Display Suite.',
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array('access administration pages'),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
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

  // Fields.
  $items['admin/structure/ds/fields'] = array(
    'title' => 'Fields',
    'description' => 'Manage custom fields for all entities.',
    'page callback' => 'ds_custom_fields',
    'file' => 'includes/ds.fields.inc',
    'access arguments' => array('admin_fields'),
  );
  $items['admin/structure/ds/fields/manage_custom'] = array(
    'title' => 'Add a field',
    'description' => 'Manage a field',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_edit_custom_field_form'),
    'file' => 'includes/ds.fields.inc',
    'access arguments' => array('admin_view_modes'),
    'type' => MENU_LOCAL_ACTION,
    'weight' => 0,
  );
  $items['admin/structure/ds/fields/manage_block'] = array(
    'title' => 'Add a block field',
    'description' => 'Manage a block field',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_edit_block_field_form'),
    'file' => 'includes/ds.fields.inc',
    'access arguments' => array('admin_view_modes'),
    'type' => MENU_LOCAL_ACTION,
    'weight' => 1,
  );
  $items['admin/structure/ds/fields/delete'] = array(
    'title' => 'Delete a field',
    'description' => 'Delete a field.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_delete_field_confirm'),
    'file' => 'includes/ds.fields.inc',
    'access arguments' => array('admin_fields'),
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
  $entities = ds_load_entity_implementations(TRUE);
  foreach ($entities as $entity_type) {
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
