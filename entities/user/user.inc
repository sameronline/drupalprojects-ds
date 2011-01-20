<?php
// $Id$

/**
 * @file
 * Display Suite user entity file.
 */

/**
 * Implements hook_ds_fields().
 *
 * @param $entity_type
 *   The name of the entity type
 * @param $bundle
 *   The name of the bundle.
 * @param $view_mode
 *   The name of the view mode.
 * @return $fields
 *   A collection of fields.
 */
function user_ds_fields($entity_type, $bundle, $view_mode) {
  $fields = array();

  // Username
  $fields['name'] = array(
    'title' => 'Username',
    'type' => DS_FIELD_TYPE_FUNCTION,
    'status' => DS_FIELD_STATUS_STATIC,
    'function' => 'ds_render_title_field',
    'properties' => array(
      'formatters' => array(
        'user_title_nolink_h1' => t('H1 title'),
        'user_title_link_h1' => t('H1 title, linked to user'),
        'user_title_nolink_h2' => t('H2 title'),
        'user_title_link_h2' => t('H2 title, linked to user'),
        'user_title_nolink_h2_block' => t('H2 block title'),
        'user_title_link_h2_block' => t('H2 block title, linked to user'),
        'user_title_nolink_p' => t('Paragraph title'),
        'user_title_link_p' => t('Paragraph title, linked to user'),
        'user_title_nolink' => t('Plain text'),
        'user_title_link' => t('Plain text, linked to user'),
      ),
    )
  );

  return array('user' => $fields);
}
