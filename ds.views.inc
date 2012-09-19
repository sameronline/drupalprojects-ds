<?php

/**
 * @file
 * Views hooks file.
 */

/**
 * Implements hook_views_plugins().
 */
function ds_views_plugins() {
  $path = drupal_get_path('module', 'ds');
  $views_plugins = array(
    'module' => 'ds',
    'row' => array(
      'ds' => array(
        'title' => t('Display Suite'),
        'help' => t('Display the entity with the Display Suite module.'),
        'handler' => 'views_plugin_ds_entity_view',
        'path' => $path . '/views',
        'base' => array('node', 'comment', 'users', 'apachesolr', 'taxonomy_term_data', 'file_managed', 'micro'),
        'theme' => 'ds_row_entity',
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );

  return $views_plugins;
}
