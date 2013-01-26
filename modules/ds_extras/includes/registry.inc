<?php

/**
 * @file
 * Display Suite Extras registry file.
 */

/**
 * Implements hook_menu_alter().
 */
function _ds_extras_menu_alter(&$items) {

  // Switch view mode.
  if (config('ds.extras')->get('switch_view_mode', FALSE)) {

    // Check if page manager is overriding.
    $skip_node_override = FALSE;
    if (module_exists('page_manager')) {
      if ($task = page_manager_get_task('node_view')) {
        if (isset($task['disabled']) && !$task['disabled']) {
          $skip_node_override = TRUE;
        }
      }
    }

    if (!$skip_node_override) {
      $items['node/%node']['page callback'] = 'ds_extras_node_page_view';
      $items['node/%node']['file'] = 'includes/pages.inc';
      $items['node/%node']['file path'] = drupal_get_path('module', 'ds_extras');
    }
  }
}

/**
 * Implements hook_module_implements_alter().
 */
function _ds_extras_module_implements_alter(&$implementations, $hook) {

  // Because it's possible to turn on/off features for DS extras,
  // we'll unset hooks here if necessary which otherwhise do nothing at all.

  // Field template
  $ft_hooks = array(
    'ds_field_settings_alter',
    'form_ds_classes_form_alter',
    'form_field_ui_field_edit_form_alter',
    'theme',
  );
  if (!config('ds.extras')->get('field_template') && in_array($hook, $ft_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Region to block
  $region_hooks = array(
    'ds_layout_region_alter',
    'field_attach_view_alter',
    'block_info',
    'block_view'
  );
  if (!config('ds.extras')->get('region_to_block', FALSE) && in_array($hook, $region_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Switch view mode
  $switch_hooks = array(
    'form_node_form_alter',
  );
  if (!config('ds.extras')->get('switch_view_mode', FALSE) && in_array($hook, $switch_hooks)) {
    unset($implementations['ds_extras']);
  }
}
