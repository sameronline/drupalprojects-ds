<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_GROUP,
  'name' => t('Fieldgroup'),
);

/**
 * Implements ds_field_api_init
 */
function ds_field_api_group_build(&$field, &$display){
  if (isset($field['format'])) {
    $field['theme'] = $field['format'];
  }
  else {
    $field['theme'] = DS_DEFAULT_THEME_FIELDSET;
  }
  $field['attributes']['class'][] = 'field-group';
}