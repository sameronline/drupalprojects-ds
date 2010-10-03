<?php
// $Id$

/**
 * @file
 * Registry file for Display Suite.
 */

/**
 * Theme implementations.
 */
function _ds_theme() {
  $theme_functions = array();

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