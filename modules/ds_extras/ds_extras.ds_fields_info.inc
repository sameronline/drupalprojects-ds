<?php

/**
 * @file
 * Display Suite extras fields.
 */

/**
 * Implements hook_ds_fields_info().
 */
function ds_extras_ds_fields_info($entity_type) {
  $fields = array();

  // Switch field support.
  if (variable_get('ds_extras_switch_field', FALSE) && $entity_type == 'node') {
    $fields[$entity_type]['ds_switch_field'] = array(
      'title' => t('Switch view mode'),
      'field_type' => DS_FIELD_TYPE_FUNCTION,
      'function' => 'ds_extras_switch_view_mode_field',
      'properties' => array('settings' => array()),
    );
  }

  // Flag support.
  if (variable_get('ds_extras_flag') && module_exists('flag')) {
    if ($entity_type == 'node') {
      $flags = flag_get_flags('node');
      foreach ($flags as $name => $flag) {
        $ui_limit = array();
        if (!empty($flag->types)) {
          foreach ($flag->types as $type) {
            $ui_limit[] = $type . '|*';
          }
        }
        $fields['node']['ds_flag_' . $name] = array(
          'title' => t('Flag: ' . $flag->get_label('title')),
          'field_type' => DS_FIELD_TYPE_FUNCTION,
          'function' => 'ds_extras_flags_add_flag_link',
          'properties' => array(
            'flag' => $name,
          ),
          'ui_limit' => $ui_limit,
        );
      }
    }
  }

  if (!empty($fields)) {
    return $fields;
  }
}
