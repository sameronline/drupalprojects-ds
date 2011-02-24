<?php
// $Id$

/**
 * Implements hook_ds_fields().
 */
function ds_extras_ds_fields($entity_type, $bundle, $view_mode) {

  if ($entity_type != 'ds_views') {
    return;
  }

  $fields = array(
    'admin_links' => array(
      'title' => t('Admin links'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,

    ),
    'title' => array(
      'title' => t('Views title'),
      'type' => DS_FIELD_TYPE_FUNCTION,
      'status' => DS_FIELD_STATUS_STATIC,
      'function' => 'ds_vd_render_title_field',
      'properties' => array(
        'formatters' => array(
          'vd_title_h1' => t('H1 title'),
          'vd_title_h2' => t('H2 title'),
          'vd_title_p' => t('Paragraph title'),
        ),
      )
    ),
    'header' => array(
      'title' => t('Views header'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'exposed' => array(
      'title' => t('Exposed filters'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'attachment_before' => array(
      'title' => t('Attachment before'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'rows' => array(
      'title' => t('Views content'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'empty' => array(
      'title' => t('Empty text'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'pager' => array(
      'title' => t('Pager'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'attachment_after' => array(
      'title' => t('Attachment after'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'more' => array(
      'title' => t('More'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'footer' => array(
      'title' => t('Views footer'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
    'feed_icon' => array(
      'title' => t('Feed icon'),
      'type' => DS_FIELD_TYPE_PREPROCESS,
      'status' => DS_FIELD_STATUS_STATIC,
    ),
  );

  return array('ds_views' => $fields);
}
