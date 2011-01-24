<?php
// $Id$

/**
 * @file
 * Display Suite node entity file.
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
function node_ds_fields($entity_type, $bundle, $view_mode) {
  $fields = array();

  // Title
  $fields['title'] = array(
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

  // Links
  // @todo test this.
  $fields['links'] = array(
    'title' => t('Links'),
    'type' => DS_FIELD_TYPE_IGNORE,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Comments.
  // @todo check in_preview.
  if (module_exists('comment') && ($view_mode == 'full' || $view_mode == 'default')) {
    $fields['comments'] = array(
      'title' => t('Comments'),
      'type' => DS_FIELD_TYPE_IGNORE,
      'status' => DS_FIELD_STATUS_STATIC,
    );
  }

  // Read more link - let's also add 'add new comment by default'
  $fields['read_more'] = array(
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

  return array('node' => $fields);
}
