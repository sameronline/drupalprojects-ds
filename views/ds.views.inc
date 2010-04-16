<?php
// $Id$

/**
 * @file
 * Views file for ds.
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
        'title' => t('Display suite node'),
        'help' => t('Display the object through the Display suite module.'),
        'handler' => 'views_plugin_ds_object_view',
        'theme' => 'ds_row_object',
        'path' => $path .'/views',
        'theme path' => $path .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
      'ds_fields' => array(
        'uses fields' => TRUE,
        'title' => t('Display suite fields'),
        'help' => t('Display fields through the Display suite module.'),
        'handler' => 'views_plugin_ds_fields_view',
        'theme' => 'ds_row_fields',
        'path' => $path .'/views',
        'theme path' => $path .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
    ),
  );
}