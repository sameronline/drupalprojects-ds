<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_CODE,
  'name' => t('Code field'),
  'formatter' => 'ds_eval_code',
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_code_build(&$field, &$display){
  if (isset($field['format'])) {
    $field['formatter'] = $field['format'];
  }
}