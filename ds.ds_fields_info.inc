<?php

/**
 * @file
 * Display Suite fields.
 */

/**
 * Implements hook_ds_fields_info().
 */
function ds_ds_fields_info($entity_type) {

  // Comments.
  if (module_exists('comment')) {
    $fields['node']['comments'] = array(
      'title' => t('Comments'),
      'field_type' => DS_FIELD_TYPE_IGNORE,
      'ui_limit' => array(
        '*|full', '*|default',
      ),
    );
  }

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
     Book support.
    -------------------------------------------------------------- */

  if (module_exists('book')) {

    $ui_limit = array();
    $types = variable_get('book_allowed_types', array('book'));
    foreach ($types as $type) {
      $ui_limit[] = $type . '|full';
    }

    if (!empty($ui_limit)) {
      $fields['node']['book_navigation'] = array(
        'title' => t('Book navigation'),
        'field_type' => DS_FIELD_TYPE_IGNORE,
        'ui_limit' => $ui_limit,
      );
    }
  }

  /* --------------------------------------------------------------
     Comment support.
    -------------------------------------------------------------- */

  if (module_exists('comment')) {

    // Comment Links.
    $fields['comment']['links'] = array(
      'title' => t('Links'),
      'field_type' => DS_FIELD_TYPE_IGNORE,
    );

    // Permalink.
    $fields['comment']['permalink'] = array(
      'title' => t('Permalink'),
      'field_type' => DS_FIELD_TYPE_PREPROCESS,
    );

    // Submitted.
    $fields['comment']['submitted'] = array(
      'title' => t('Submitted'),
      'field_type' => DS_FIELD_TYPE_PREPROCESS,
    );

    // Title.
    $fields['comment']['title'] = array(
      'title' => t('Title'),
      'field_type' => DS_FIELD_TYPE_PREPROCESS,
    );

    // Author.
    $fields['comment']['author'] = array(
      'title' => t('Author'),
      'field_type' => DS_FIELD_TYPE_PREPROCESS,
    );

    // User signature.
    if (variable_get('user_signatures', 0)) {
      $fields['comment']['signature'] = array(
        'title' => t('User signature'),
        'field_type' => DS_FIELD_TYPE_PREPROCESS,
      );
    }

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

  // User signature
  if (variable_get('user_signatures', 0)) {
    $fields['user']['user_signature'] = array(
      'title' => t('User signature'),
      'field_type' => DS_FIELD_TYPE_FUNCTION,
      'function' => 'ds_render_markup',
      'properties' => array(
        'key' => 'signature',
        'format' => 'signature_format',
      ),
    );
  }

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

  /* --------------------------------------------------------------
     Taxonomy support.
    -------------------------------------------------------------- */

  if (module_exists('taxonomy')) {

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

  if (isset($fields[$entity_type])) {
    return array($entity_type => $fields[$entity_type]);
  }
  return;
}
