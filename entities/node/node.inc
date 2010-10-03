<?php
// $Id$

/**
 * @file
 * Display Suite node entity base file.
 */

/**
 * Implements hook_ds_theme_registry().
 */
function node_ds_theme_registry() {
  $theme_functions = array();

  // Single theming functions.
  $formatters = array(
    'nd_bodyfield', 'nd_title_h1_nolink', 'nd_title_h1_link', 'nd_title_h2_nolink', 'nd_title_h2_link',
    'nd_title_h2_block_nolink', 'nd_title_h2_block_link', 'nd_title_p_nolink', 'nd_title_p_link',
    'nd_book_add_new_child', 'nd_book_printer_version',
  );

  foreach ($formatters as $formatter) {
    $theme_functions[$formatter] = array(
      'variables' => array('node' => NULL),
      'file' => 'entities/node/node.theme.inc',
    );
  }

  return $theme_functions;
}

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

  // Title TODO check if type has a title.
  $fields['title'] = array(
    'title' => t('Title'),
    'type' => DS_FIELD_TYPE_THEME,
    'status' => DS_FIELD_STATUS_STATIC,
    'properties' => array(
      'formatters' => array(
        'nd_title_h1_nolink' => t('H1 title'),
        'nd_title_h1_link' => t('H1 title, linked to node'),
        'nd_title_h2_nolink' => t('H2 title'),
        'nd_title_h2_link' => t('H2 title, linked to node'),
        'nd_title_h2_block_nolink' => t('H2 block title'),
        'nd_title_h2_block_link' => t('H2 block title, linked to node'),
        'nd_title_p_nolink' => t('Paragraph title'),
        'nd_title_p_link' => t('Paragraph title, linked to node'),
      ),
    )
  );

  // Author
  $fields['author'] = array(
    'title' => t('Author'),
    'type' => DS_FIELD_TYPE_THEME,
    'status' => DS_FIELD_STATUS_STATIC,
    'properties' => array(
      'formatters' => array(
        'ds_author_nolink' => t('Author'),
        'ds_author_link' => t('Author linked to profile')
      ),
    ),
  );

  // Links.
  $fields['links'] = array(
    'title' => t('Links'),
    'type' => DS_FIELD_TYPE_PREPROCESS,
    'status' => DS_FIELD_STATUS_STATIC,
  );

  // Read more link.
  $fields['read_more'] = array(
    'title' => t('Read more'),
    'type' => DS_FIELD_TYPE_CODE,
    'status' => DS_FIELD_STATUS_DEFAULT,
    'properties' => array(
      'formatters' => array(
        'ds_eval_code' => t('Default'),
      ),
      'code' => '<?php echo l(t("Read more"), "node/$object->nid"); ?>' // TODO theme this no ? // or use new tokens in Drupal Core
    ),
  );

  return array('node' => $fields);
}
