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
  if (Drupal::config('ds.extras')->get('switch_view_mode')) {

    // Check if page manager is overriding.
    $skip_node_override = FALSE;
    if (Drupal::moduleHandler()->moduleExists('page_manager')) {
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

  // Region to block
  $region_hooks = array(
    'ds_layout_region_alter',
    'entity_view_alter',
  );
  if (!Drupal::config('ds.extras')->get('region_to_block') && in_array($hook, $region_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Extra fields
  $extra_fields_hooks = array(
    'field_extra_fields',
  );
  if (!Drupal::config('ds.extras')->get('fields_extra') && in_array($hook, $extra_fields_hooks)) {
    unset($implementations['ds_extras']);
  }

}
