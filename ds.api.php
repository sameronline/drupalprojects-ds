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
 *   A collection of fields which keys are the entity type name and values
 *   a collection fields.
 *
 * @see ds_get_fields()
 */
function hook_ds_fields($entity_type, $bundle, $view_mode) {
  $fields = array();

  $fields['title'] = array(

    // title: title of the field
    'title' => t('Title'),

    // type: type of field
    // - DS_FIELD_TYPE_THEME : calls a theming function
    // - DS_FIELD_TYPE_FUNCTION : calls a custom function
    // - DS_FIELD_TYPE_CODE : calls theme_ds_eval_code
    // - DS_FIELD_TYPE_BLOCK : calls theme_eval_block.
    'type' => DS_FIELD_TYPE_FUNCTION,

    // file: an optional file in which the function resides.
    // Only for DS_FIELD_TYPE_FUNCTION.
    'file' => 'optional_filename',

    // status: status of the field.
    // - DS_FIELD_STATUS_STATIC : static field
    // - DS_FIELD_STATUS_DEFAULT : default field
    'status' => DS_FIELD_STATUS_STATIC,

    // function: only for DS_FIELD_TYPE_FUNCTION.
    'function' => 'theme_ds_title_field',

    // properties: can have different keys.
    'properties' => array(

      // formatters: optional if a a function is used.
      'formatters' => array(
        'node_title_nolink_h1' => t('H1 title'),
        'node_title_link_h1' => t('H1 title, linked to node'),
      ),

      // code: optional, only for code field.
      'code' => 'my code here',

      // use_token: optional, only for code field.
      'code' => TRUE, // or FALSE,

      // block: the module and delta of the block, only for block fields.
      'block' => 'user-menu',

      // block_render: block render type, only for block fields.
      // - DS_BLOCK_CONTENT : render through block template file.
      // - DS_BLOCK_TITLE_CONTENT : render only title and content.
      // - DS_BLOCK_CONTENT : render only content.
      'block_render' => DS_BLOCK_CONTENT,
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
