<?php

/**
 * @file
 * Display Suite fields.
 */

function system_get_date_formats() {
  // TODO replace me
  return array();
}

/**
 * Implements hook_ds_fields_info().
 */
function ds_ds_fields_info($entity_type) {

  // User picture
  if (variable_get('user_pictures', 0)) {
    $key = 'user_picture';
    $type = DS_FIELD_TYPE_IGNORE;
    $picture_formatters = array();
    if (module_exists('image')) {
      $key = 'ds_user_picture';
      $type = DS_FIELD_TYPE_FUNCTION;
      $styles =  entity_load_multiple('image_style');
      foreach ($styles as $style) {
        $picture_formatters['ds_picture_' . $style->name] = $style->label;
      }
    }
    else {
      $picture_formatters['default'] = t('Default');
    }
    $fields['node'][$key] = array(
      'title' => t('User picture'),
      'field_type' => $type,
      'properties' => array(
        'formatters' => $picture_formatters,
      ),
    );
    if ($type == DS_FIELD_TYPE_FUNCTION) {
      $fields['node'][$key]['function'] = 'ds_render_user_picture';
    }
  }

  /* --------------------------------------------------------------
     Comment support.
    -------------------------------------------------------------- */

  if (module_exists('comment')) {
    // User picture
    if (variable_get('user_pictures', 0)) {
      $key = 'picture';
      $type = DS_FIELD_TYPE_PREPROCESS;
      $picture_formatters = array();
      if (module_exists('image')) {
        $key = 'ds_user_picture';
        $type = DS_FIELD_TYPE_FUNCTION;
        $styles =  entity_load_multiple('image_style');
        foreach ($styles as $style) {
          $picture_formatters['ds_picture_' . $style->name] = $style->label;
        }
      }
      else {
        $picture_formatters['default'] = t('Default');
      }
      $fields['comment'][$key] = array(
        'title' => t('User picture'),
        'field_type' => $type,
        'properties' => array(
          'formatters' => $picture_formatters,
        ),
      );
      if ($type == DS_FIELD_TYPE_FUNCTION) {
        $fields['comment'][$key]['function'] = 'ds_render_user_picture';
      }
    }
  }

  /* --------------------------------------------------------------
     User support.
    -------------------------------------------------------------- */

  // User picture
  if (variable_get('user_pictures', 0)) {
    $key = 'user_picture';
    $type = DS_FIELD_TYPE_IGNORE;
    $picture_formatters = array();
    if (module_exists('image')) {
      $key = 'ds_user_picture';
      $type = DS_FIELD_TYPE_FUNCTION;
      $styles =  entity_load_multiple('image_style');
      foreach ($styles as $style) {
        $picture_formatters['ds_picture_' . $style->name] = $style->label;
      }
    }
    else {
      $picture_formatters['default'] = t('Default');
    }
    $fields['user'][$key] = array(
      'title' => t('User picture'),
      'field_type' => $type,
      'properties' => array(
        'formatters' => $picture_formatters,
      ),
    );
    if ($type == DS_FIELD_TYPE_FUNCTION) {
      $fields['user'][$key]['function'] = 'ds_render_user_picture';
    }
  }


  // Support for ECK Entity title
  if (module_exists('eck')) {
    $entity_info = entity_get_info($entity_type);
    if (isset($entity_info['module']) && $entity_info['module'] == 'eck') {
      $fields[$entity_type]['title'] = array(
        'title' => t('Title'),
        'field_type' => DS_FIELD_TYPE_FUNCTION,
        'function' => 'ds_render_field',
        'properties' => array(
          'entity_render_key' => 'title',
          'settings' => array(
            'link' => array('type' => 'select', 'options' => array('no', 'yes')),
            'wrapper' => array('type' => 'textfield', 'description' => t('Eg: h1, h2, p')),
            'class' => array('type' => 'textfield', 'description' => t('Put a class on the wrapper. Eg: block-title')),
          ),
          'default' => array('wrapper' => 'h2', 'link' => 0, 'class' => ''),
        )
      );
    }
  }

}
