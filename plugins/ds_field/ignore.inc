<?php
/**
 * @file
 * The Display Suite field type ignore plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_IGNORE,
  'name' => t('Available in object content'),
  'pipeline' => DS_RENDER_NON_DS,
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_ignore_build(&$field, $display){
  if (isset($field['properties']['key']) && !empty($field['properties']['key'])) {
    $field['preprocess_settings'] = array('type' => $field['type'], 'key' => $field['properties']['key']);
  }
  else {
    $field['preprocess_settings'] = array('type' => $field['type']);
  }
}