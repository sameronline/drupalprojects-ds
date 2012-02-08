<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_THEME,
  'name' => t('Theme field'),
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_theme_build(&$field){
  $field['formatter'] = (isset($field['format'])) ? $field['format'] : key($field['properties']['formatters']);
}