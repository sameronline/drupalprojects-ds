<?php
/**
 * @file
 * The Display Suite field type non-DS plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_NON_DS,
  'name' => t('Non-DS field'),
  'pipeline' => DS_RENDER_NON_DS,
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_non_ds_init(&$field){
  $field['foo'] = 'bar';
}