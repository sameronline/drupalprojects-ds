<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_PREPROCESS,
  'name' => t('Preprocess field'),
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_preprocess_build(&$field, &$display){
  if (isset($field['properties']['key']) && !empty($field['properties']['key'])) {
    $field['preprocess_settings'] = array('type' => $field['type'], 'key' => $field['properties']['key']);
  }
  else {
    $field['preprocess_settings'] = array('type' => $field['type']);
  }
}