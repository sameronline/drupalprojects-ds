<?php
// $Id$

/**
 * @file
 * Views plugins provided by Display Suite.
 */

/**
 * Implementation of hook_views_plugins().
 */
function ds_views_plugins() {
  $path = drupal_get_path('module', 'ds');
  return array(
    'module' => 'ds',
    'row' => array(
      'ds' => array(
        'title' => t('Display suite entity'),
        'help' => t('Display the entity with the Display suite module.'),
        'handler' => 'views_plugin_ds_entity_view',
        'path' => $path .'/views',
        'theme' => 'ds_row_entity',
        'theme path' => $path .'/views',
        'theme file' => 'views.theme.inc',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-entity',
      ),
    ),
  );
}