<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_FUNCTION,
  'name' => t('Custom function'),
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_function_init(&$field){
  $field['function'] = (isset($field['format'])) ? $field['format'] : key($field['properties']['formatters']);
}