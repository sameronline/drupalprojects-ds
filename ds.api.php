<?php
// $Id$

/**
 * @file
 * Hooks provided by Display Suite module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define custom fields.
 *
 * @param $entity_type
 *   The name of the entity which we are requesting fields for, eg 'node'.
 * @param $bundle
 *   The name of the bundle in the entity, eg 'article'.
 * @param $view_mode
 *   The name of the view mode, eg 'full'.
 *
 *
 * @return $fields
 *   A collection of fields which keys are the entity type name and value
 *   a collection field which keys are the field name and value a collection
 *   of field properties.
 *
 * @see ds_get_fields()
 */
function hook_ds_fields($entity_type, $bundle, $view_mode) {
  $fields = array();

  // @todo document the keys above.
  $fields['title'] = array(
    'title' => t('Title'),
    // Type can either be
    // - DS_FIELD_TYPE_THEME : calls a theming function
    // - DS_FIELD_TYPE_FUNCTION : calls a custom function
    // - DS_FIELD_TYPE_CODE : calls theme_ds_eval_code
    // - DS_FIELD_TYPE_BLOCK : calls theme_eval_block.
    'type' => DS_FIELD_TYPE_FUNCTION,
    // File is an optional file in which the function resides.
    // Only for DS_FIELD_TYPE_FUNCTION.
    'file' => 'optional_filename',
    'status' => DS_FIELD_STATUS_STATIC,
    // Function name, only for DS_FIELD_TYPE_FUNCTION.
    'function' => 'theme_ds_title_field',
    'properties' => array(
      'formatters' => array(
        'node_title_nolink_h1' => t('H1 title'),
        'node_title_link_h1' => t('H1 title, linked to node'),
      ),
    )
  );

  return array('node' => $fields);

}

/**
 * Alter fields defined by Display Suite
 *
 * @param $fields
 *   An array with fields which can altered just before they are cached.
 */
function hook_ds_fields_alter(&$fields) {
  if (isset($fields['title'])) {
    $fields['title']['title'] = t('My title');
  }
}

/**
 * Define layouts from code.
 *
 * @return $layouts
 *   A collection of layouts.
 */
function hook_ds_layouts() {
  $path = drupal_get_path('module', 'foo');

  $layouts = array(
    'foo_1col' => array(
      'label' => t('Foo one column'),
      'path' => $path . '/layouts/foo_1col',
      'regions' => array(
        'foo_content' => t('Content'),
      ),
    ),
  );

  return $layouts;
}

/**
 * Theme an entity coming from the views entity plugin.
 *
 * @param $vars
 *   An array of variables from the views preprocess functions.
 * @param $view_mode
 *   The name of the view mode.
 */
function ds_views_row_ENTITY_NAME(&$vars, $view_mode) {
  $nid = $vars['row']->{$vars['field_alias']};
  $node = node_load($nid);
  $vars['object'] = drupal_render(node_view($node, $view_mode));
}

/**
 * Theme an entity through an advanced function coming from the views entity plugin.
 *
 * @param $vars
 *   An array of variables from the views preprocess functions.
 * @param $view_mode
 *   The name of the view mode.
 */
function ds_views_row_adv_VIEWS_NAME(&$vars, $view_mode) {
  // You can do whatever you want to here.
  $vars['object'] = 'This is what I want for christmas.';
}

/**
 * @} End of "addtogroup hooks".
 */
