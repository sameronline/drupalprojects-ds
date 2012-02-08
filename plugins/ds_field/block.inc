<?php
/**
 * @file
 * The Display Suite field type block plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_BLOCK,
  'name' => t('Block field'),
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_block_init(&$field){
  $field['formatter'] = 'ds_eval_block';
}