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
    'description' => 'Manage fields for all entities.',
    'page callback' => 'ds_custom_fields',
    'file' => 'includes/ds.fields.inc',
    'access arguments' => array('admin_fields'),
  );
  $items['admin/structure/ds/fields/manage'] = array(
    'title' => 'Fields edit',
    'page callback' => 'ds_custom_manage',
    'file' => 'includes/ds.fields.inc',
    'access arguments' => array('admin_fields'),
    'type' => MENU_CALLBACK,
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
  // Block can be disabled.
  if (module_exists('block')) {
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
  }
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
    // Layouts
    // @todo should be registered otherwise of course.
    'ds_2col' => array(
      'render element' => 'elements',
      'template' => 'layouts/ds_2col/ds-2col',
    ),
    'ds_2col_stacked' => array(
      'render element' => 'elements',
      'template' => 'layouts/ds_2col_stacked/ds-2col-stacked',
    ),
  );

  // Load theming functions from entities we implement.
  $entities = ds_load_entity_implementations(TRUE);
  foreach ($entities as $entity_type) {
    $entity_type_theme_function = $entity_type . '_ds_theme_registry';
    $theme_functions += $entity_type_theme_function();
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

  // Add layout if applicable.
  $ds_layouts = module_invoke_all('ds_layouts');
  $results = db_query('SELECT entity_type, bundle, view_mode, layout, load_css, load_template FROM {ds_layouts}')->fetchAll();
  foreach ($results as $row) {
    $layout = $ds_layouts[$row->layout];
    $layout['layout'] = $row->layout;
    $layout['load_css'] = $row->load_css;
    $layout['load_template'] = $row->load_template;
    $entity_info[$row->entity_type]['bundles'][$row->bundle]['layouts'][$row->view_mode] = $layout;
  }
}

/**
 * Display suite layouts.
 */
function _ds_layouts() {
  $path = drupal_get_path('module', 'ds');
  $layouts = array(
    'ds_2col' => array(
      'label' => t('Two column'),
      'path' => $path . '/layouts/ds_2col',
      'regions' => array(
        'left' => t('Left'),
        'right' => t('Right')
      ),
    ),
    'ds_2col_stacked' => array(
      'label' => t('Two column stacked'),
      'path' => $path . '/layouts/ds_2col_stacked',
      'regions' => array(
        'header' => t('Header'),
        'left' => t('Left'),
        'right' => t('Right'),
        'footer' => t('Footer'),
      ),
    ),
    'ds_3col' => array(
      'label' => t('Three column'),
      'path' => $path . '/layouts/ds_3col',
      'regions' => array(
        'left' => t('Left'),
        'middle' => t('Middle'),
        'right' => t('Right'),
      ),
    ),
    'ds_3col_stacked' => array(
      'label' => t('Three column stacked'),
      'path' => $path . '/layouts/ds_3col_stacked',
      'regions' => array(
        'header' => t('Header'),
        'left' => t('Left'),
        'middle' => t('Middle'),
        'right' => t('Right'),
        'footer' => t('Footer'),
      ),
    ),
  );

  return $layouts;
}
