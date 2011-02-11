<?php
// $Id$

/**
 * @file
 * Display suite fields
 */

/**
 * Implements hook_ds_fields().
 */
function ds_ds_fields($entity_type, $bundle, $view_mode) {

  // @todo cleanup, maybe keep static etc.

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
    'function' => 'ds_render_title_field',
    'properties' => array(
      'formatters' => array(
        'node_title_nolink_h1' => t('H1 title'),
        'node_title_link_h1' => t('H1 title, linked to node'),
        'node_title_nolink_h2' => t('H2 title'),
        'node_title_link_h2' => t('H2 title, linked to node'),
        'node_title_nolink_h2_block' => t('H2 block title'),
        'node_title_link_h2_block' => t('H2 block title, linked to node'),
        'node_title_nolink_p' => t('Paragraph title'),
        'node_title_link_p' => t('Paragraph title, linked to node'),
        'node_title_nolink' => t('Plain text'),
        'node_title_link' => t('Plain text, linked to node'),
      ),
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

  // Read more link.
  $fields['node']['read_more'] = array(
    'title' => t('Read more'),
    'type' => DS_FIELD_TYPE_CODE,
    'status' => DS_FIELD_STATUS_DEFAULT,
    'properties' => array(
      'code' => array(
        'value' => '<?php echo l("Read more", "node/" . $entity->nid); ?>',
        'format' => 'ds_code',
      ),
    ),
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
    'function' => 'ds_render_title_field',
    'properties' => array(
      'formatters' => array(
        'user_name_nolink_h1' => t('H1 title'),
        'user_name_link_h1' => t('H1 title, linked to user'),
        'user_name_nolink_h2' => t('H2 title'),
        'user_name_link_h2' => t('H2 title, linked to user'),
        'user_name_nolink_h2_block' => t('H2 block title'),
        'user_name_link_h2_block' => t('H2 block title, linked to user'),
        'user_name_nolink_p' => t('Paragraph title'),
        'user_name_link_p' => t('Paragraph title, linked to user'),
        'user_name_nolink' => t('Plain text'),
        'user_name_link' => t('Plain text, linked to user'),
      ),
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
    'function' => 'ds_render_title_field',
    'properties' => array(
      'formatters' => array(
        'tax_name_nolink_h1' => t('H1 title'),
        'tax_name_link_h1' => t('H1 title, linked to term'),
        'tax_name_nolink_h2' => t('H2 title'),
        'tax_name_link_h2' => t('H2 title, linked to term'),
        'tax_name_nolink_h2_block' => t('H2 block title'),
        'tax_name_link_h2_block' => t('H2 block title, linked to term'),
        'tax_name_nolink_p' => t('Paragraph title'),
        'tax_name_link_p' => t('Paragraph title, linked to term'),
        'tax_name_nolink' => t('Plain text'),
        'tax_name_link' => t('Plain text, linked to term'),
      ),
    )
  );

  if (isset($fields[$entity_type])) {
    return array($entity_type => $fields[$entity_type]);
  }
  return;
}