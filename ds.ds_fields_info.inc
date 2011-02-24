<?php
// $Id$

/**
 * @file
 * Display suite fields
 */

/**
 * Implements hook_ds_fields_info().
 */
function ds_ds_fields_info($entity_type, $bundle, $view_mode) {

  $custom_fields = ds_custom_fields();
  foreach ($custom_fields as $key => $field) {
    if (isset($field->entities[$entity_type])) {
      $fields[$entity_type][$key] = array(
        'title' => $field->label,
        'type' => $field->type,
        'properties' => (array) $field->properties,
      );
    }
  }

  // Node title.
  $fields['node']['title'] = array(
    'title' => t('Title'),
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_field',
    'properties' => array(
      'entity_path' => 'node',
      'entity_id_key' => 'nid',
      'entity_render_key' => 'title',
      'settings' => array(
        'link' => array('type' => 'select', 'options' => array('no', 'yes')),
        'wrapper' => array('type' => 'textfield', 'description' => t('Eg: h1, h2, p')),
        'class' => array('type' => 'textfield', 'description' => t('Put a class on the wrapper. Eg: block-title')),
      ),
      'default' => array('wrapper' => 'h2', 'link' => 0, 'class' => ''),
    )
  );

  // Links.
  $fields['node']['links'] = array(
    'title' => t('Links'),
    'type' => DS_FIELD_TYPE_IGNORE,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Comments.
  if (module_exists('comment') && ($view_mode == 'full' || $view_mode == 'default')) {
    $fields['node']['comments'] = array(
      'title' => t('Comments'),
      'type' => DS_FIELD_TYPE_IGNORE,
      'status' => DS_FIELD_STATUS_STATIC,
    );
  }

  // Node link.
  $fields['node']['node_link'] = array(
    'title' => t('Read more'),
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_field',
    'properties' => array(
      'entity_path' => 'node',
      'entity_id_key' => 'nid',
      'settings' => array(
        'link text' => array('type' => 'textfield', 'options' => array('no', 'yes')),
        'wrapper' => array('type' => 'textfield', 'description' => t('Eg: h1, h2, p')),
        'class' => array('type' => 'textfield', 'description' => t('Put a class on the wrapper. Eg: block-title')),
      ),
      'default' => array('link text' => 'Read more', 'wrapper' => '', 'class' => '', 'link' => 1),
    )
  );

  // Author.
  $fields['node']['author'] = array(
    'title' => t('Author'),
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_author_field',
    'properties' => array(
      'formatters' => array(
        'author' => t('Author'),
        'author_linked' => t('Author linked to profile')
      ),
    ),
  );

  // Created time.
  $format_types = system_get_date_types();
  $date_formatters = array();
  foreach ($format_types as $formatter) {
    $date_formatters['ds_post_date_'. $formatter['type']] = t($formatter['title']);
  }
  $fields['node']['post_date'] = array(
    'title' => t('Post date'),
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_date_field',
    'properties' => array(
      'formatters' => $date_formatters,
    ),
  );

  // User picture
  if (variable_get('user_pictures', 0)) {
    $key = 'user_picture';
    $type = DS_FIELD_TYPE_IGNORE;
    $picture_formatters = array();
    if (module_exists('image')) {
      $key = 'ds_user_picture';
      $type = DS_FIELD_TYPE_FUNCTION;
      $styles = image_styles();
      foreach ($styles as $formatter) {
        $picture_formatters['ds_picture_'. $formatter['name']] = ucfirst(str_replace('_', ' ', $formatter['name']));
      }
    }
    else {
      $picture_formatters['default'] = t('Default');
    }
    $fields['node'][$key] = array(
      'title' => t('User picture'),
      'type' => $type,
      'status' => DS_FIELD_STATUS_STATIC,
      'properties' => array(
        'formatters' => $picture_formatters,
      ),
    );
    if ($type == DS_FIELD_TYPE_FUNCTION) {
      $fields['node'][$key]['function'] = 'ds_render_user_picture';
    }
  }

  // Comment Links.
  $fields['comment']['links'] = array(
    'title' => t('Links'),
    'type' => DS_FIELD_TYPE_IGNORE,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Created time.
  $format_types = system_get_date_types();
  $date_formatters = array();
  foreach ($format_types as $formatter) {
    $date_formatters['ds_post_date_'. $formatter['type']] = t($formatter['title']);
  }
  $fields['comment']['post_date'] = array(
    'title' => t('Post date'),
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_date_field',
    'properties' => array(
      'formatters' => $date_formatters,
    ),
  );

  // Permalink.
  $fields['comment']['permalink'] = array(
    'title' => t('Permalink'),
    'type' => DS_FIELD_TYPE_PREPROCESS,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Submitted.
  $fields['comment']['submitted'] = array(
    'title' => t('Submitted'),
    'type' => DS_FIELD_TYPE_PREPROCESS,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Title.
  $fields['comment']['title'] = array(
    'title' => t('Title'),
    'type' => DS_FIELD_TYPE_PREPROCESS,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Author.
  $fields['comment']['author'] = array(
    'title' => t('Author'),
    'type' => DS_FIELD_TYPE_PREPROCESS,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // User signature.
  if (variable_get('user_signatures', 0)) {
    $fields['comment']['signature'] = array(
      'title' => t('User signature'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
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
      $styles = image_styles();
      foreach ($styles as $formatter) {
        $picture_formatters['ds_picture_'. $formatter['name']] = ucfirst(str_replace('_', ' ', $formatter['name']));
      }
    }
    else {
      $picture_formatters['default'] = t('Default');
    }
    $fields['comment'][$key] = array(
      'title' => t('User picture'),
      'type' => $type,
      'status' => DS_FIELD_STATUS_STATIC,
      'properties' => array(
        'formatters' => $picture_formatters,
      ),
    );
    if ($type == DS_FIELD_TYPE_FUNCTION) {
      $fields['comment'][$key]['function'] = 'ds_render_user_picture';
    }
  }

  // Username
  $fields['user']['name'] = array(
    'title' => 'Username',
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_field',
    'properties' => array(
      'entity_path' => 'user',
      'entity_id_key' => 'uid',
      'entity_render_key' => 'name',
      'settings' => array(
        'link' => array('type' => 'select', 'options' => array('no', 'yes')),
        'wrapper' => array('type' => 'textfield', 'description' => t('Eg: h1, h2, p')),
        'class' => array('type' => 'textfield', 'description' => t('Put a class on the wrapper. Eg: block-title')),
      ),
      'default' => array('wrapper' => 'h2', 'link' => 0, 'class' => ''),
    )
  );

  // User signature
  if (variable_get('user_signatures', 0)) {
    $fields['user']['user_signature'] = array(
      'title' => t('User signature'),
      'type' => DS_FIELD_TYPE_FUNCTION,
      'status' => DS_FIELD_STATUS_STATIC,
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
      $styles = image_styles();
      foreach ($styles as $formatter) {
        $picture_formatters['ds_picture_'. $formatter['name']] = ucfirst(str_replace('_', ' ', $formatter['name']));
      }
    }
    else {
      $picture_formatters['default'] = t('Default');
    }
    $fields['user'][$key] = array(
      'title' => t('User picture'),
      'type' => $type,
      'status' => DS_FIELD_STATUS_STATIC,
      'properties' => array(
        'formatters' => $picture_formatters,
      ),
    );
    if ($type == DS_FIELD_TYPE_FUNCTION) {
      $fields['user'][$key]['function'] = 'ds_render_user_picture';
    }
  }

  // Taxonomy term title.
  $fields['taxonomy_term']['title'] = array(
    'title' => t('Title'),
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_field',
    'properties' => array(
      'entity_path' => 'taxonomy/term',
      'entity_id_key' => 'tid',
      'entity_render_key' => 'name',
      'settings' => array(
        'link' => array('type' => 'select', 'options' => array('no', 'yes')),
        'wrapper' => array('type' => 'textfield', 'description' => t('Eg: h1, h2, p')),
        'class' => array('type' => 'textfield', 'description' => t('Put a class on the wrapper. Eg: block-title')),
      ),
      'default' => array('wrapper' => 'h2', 'link' => 0, 'class' => ''),
    )
  );

  if (isset($fields[$entity_type])) {
    return array($entity_type => $fields[$entity_type]);
  }
  return;
}
