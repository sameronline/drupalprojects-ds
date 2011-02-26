<?php

/**
 * @file
 * Hooks provided by Display Suite module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hook_test_ctools_plugin_api($module, $api) {
  if ($module == 'ds' && $api == 'ds') {
    return array('version' => 1);
  }
}

/**
 * Expose Display suite field settings.
 *
 * These hooks are called by CTools. For this hook to work, you need
 * to implement hook_ctools_plugin_api(). The values of these hooks
 * can be overridden and reverted through the UI.
 */
function hook_ds_field_settings_info() {
  $dsfieldsettings = array();

  $dsfieldsetting = new stdClass;
  $dsfieldsetting->disabled = FALSE; /* Edit this to true to make a default dsfieldsetting disabled initially */
  $dsfieldsetting->api_version = 1;
  $dsfieldsetting->id = 'node|article|default';
  $dsfieldsetting->entity_type = 'node';
  $dsfieldsetting->bundle = 'article';
  $dsfieldsetting->view_mode = 'default';
  $dsfieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h3',
        'class' => '',
      ),
    ),
    'node_link' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $dsfieldsettings['node|article|default'] = $dsfieldsetting;

  return $dsfieldsettings;
}

/**
 * Expose default layout settings info.
 *
 * These hooks are called by CTools. For this hook to work, you need
 * to implement hook_ctools_plugin_api(). The values of these hooks
 * can be overridden and reverted through the UI.
 */
function hook_ds_layout_settings_info() {
  $dslayouts = array();

  $dslayout = new stdClass;
  $dslayout->disabled = FALSE; /* Edit this to true to make a default dslayout disabled initially */
  $dslayout->api_version = 1;
  $dslayout->id = 'node|article|default';
  $dslayout->entity_type = 'node';
  $dslayout->bundle = 'article';
  $dslayout->view_mode = 'default';
  $dslayout->layout = 'ds_2col';
  $dslayout->settings = array(
    'hide_empty_regions' => 0,
    'regions' => array(
      'left' => array(
        0 => 'title',
        1 => 'node_link',
      ),
      'right' => array(
        0 => 'body',
      ),
    ),
    'fields' => array(
      'title' => 'left',
      'node_link' => 'left',
      'body' => 'right',
    ),
    'classes' => array(),
  );
  $dslayouts['node|article|default'] = $dslayout;

  return $dslayouts;
}

/**
 * Expose default view modes.
 *
 * These hooks are called by CTools. For this hook to work, you need
 * to implement hook_ctools_plugin_api(). The values of these hooks
 * can be overridden and reverted through the UI.
 */
function hook_ds_view_modes_info() {
  $ds_view_modes = array();

  $ds_view_mode = new stdClass;
  $ds_view_mode->disabled = FALSE; /* Edit this to true to make a default ds_view_mode disabled initially */
  $ds_view_mode->api_version = 1;
  $ds_view_mode->view_mode = 'test_exportables';
  $ds_view_mode->label = 'Test exportables';
  $ds_view_mode->entities = array(
    'node' => 'node',
  );
  $ds_view_modes['test_exportables'] = $ds_view_mode;

  return $ds_view_modes;
}

/**
 * Define custom fields.
 *
 * @param $entity_type
 *   The name of the entity which we are requesting fields for, e.g. 'node'.
 * @param $bundle
 *   The name of the bundle in the entity, e.g. 'article'.
 * @param $view_mode
 *   The name of the view mode, e.g. 'full'.
 *
 *
 * @return $fields
 *   A collection of fields which keys are the entity type name and values
 *   a collection fields.
 *
 * @see ds_get_fields()
 */
function hook_ds_fields_info($entity_type, $bundle, $view_mode) {
  $fields = array();

  $fields['title'] = array(

    // title: title of the field
    'title' => t('Title'),

    // type: type of field
    // - DS_FIELD_TYPE_THEME    : calls a theming function.
    // - DS_FIELD_TYPE_FUNCTION : calls a custom function.
    // - DS_FIELD_TYPE_CODE     : calls ds_render_code_field().
    // - DS_FIELD_TYPE_BLOCK    : calls ds_render_block_field().
    // - DS_FIELD_TYPE_IGNORE   : calls nothing, use this if you simple want
    //                            to drag and drop. The field itself will have
    //                            a theme function.
    'type' => DS_FIELD_TYPE_FUNCTION,

    // file: an optional file in which the function resides.
    // Only for DS_FIELD_TYPE_FUNCTION.
    'file' => 'optional_filename',

    // function: only for DS_FIELD_TYPE_FUNCTION.
    'function' => 'theme_ds_title_field',

    // properties: can have different keys.
    'properties' => array(

      // formatters: optional if a a function is used.
      'formatters' => array(
        'node_title_nolink_h1' => t('H1 title'),
        'node_title_link_h1' => t('H1 title, linked to node'),
      ),

      // settings & default: optional if you have a settings form for your field.
      'settings' => array(
        'wrapper' => array('type' => 'textfield', 'description' => t('Eg: h1, h2, p')),
        'link' => array('type' => 'select', 'options' => array('yes', 'no')),
      ),
      'default' => array('wrapper' => 'h2', 'link' => 0),

      // code: optional, only for code field.
      'code' => 'my code here',

      // use_token: optional, only for code field.
      'code' => TRUE, // or FALSE,

      // block: the module and delta of the block, only for block fields.
      'block' => 'user-menu',

      // block_render: block render type, only for block fields.
      // - DS_BLOCK_CONTENT       : render through block template file.
      // - DS_BLOCK_TITLE_CONTENT : render only title and content.
      // - DS_BLOCK_CONTENT       : render only content.
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
function hook_ds_fields_info_alter(&$fields) {
  if (isset($fields['title'])) {
    $fields['title']['title'] = t('My title');
  }
}

/**
 * Creates a summary for the field format configuration summary.
 *
 * As soon as you implement hook_ds_fields() and one of the fields
 * has a settings key, Display Suite will call this hook for the summary.
 *
 * @param $field
 *   The configuration of the field.
 *
 * @return $summary
 *   The summary to show on the Field UI.
 */
function hook_ds_field_settings_summary($field) {
  return 'Field summary';
}

/**
 * Return a settings form for a Display Suite field.
 *
 * As soon as you implement hook_ds_fields() and one of the fields
 * has a settings key, Display Suite will call this hook for field form.
 *
 * @param $field
 *   The configuration of the field.
 *
 * @return $form
 *   A form definition.
 */
function hook_ds_field_settings_form($field) {

  // Saved formatter settings are on $field['formatter_settings'];
  $settings = isset($field['formatter_settings']) ? $field['formatter_settings'] : $field['properties']['default'];

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $settings['label'],
  );
}

/**
 * Define layouts from code.
 *
 * @return $layouts
 *   A collection of layouts.
 */
function hook_ds_layout_info() {
  $path = drupal_get_path('module', 'foo');

  $layouts = array(
    'foo_1col' => array(
      'label' => t('Foo one column'),
      'path' => $path . '/layouts/foo_1col',
      'regions' => array(
        'foo_content' => t('Content'),
      ),
      'css' => TRUE,
    ),
  );

  return $layouts;
}

/**
 * Alter the region options in the field UI screen.
 *
 * This function is only called when a layout has been chosen.
 *
 * @param $context
 *   A collection of keys for the context. The keys are 'entity_type',
 *   'bundle' and 'view_mode'.
 * @param $region_info
 *   A collection of info for regions. The keys are 'region_options'
 *   and 'table_regions'.
 */
function hook_ds_layout_region_alter($context, &$region_info) {
  $region_info['region_options'][$block_key] = $block['title'];
  $region_info['table_regions'][$block_key] = array(
    'title' => check_plain($block['title']),
    'message' => t('No fields are displayed in this region'),
  );
}

/**
 * Themes can also define extra layouts.
 *
 * Create a ds_layouts folder and then a folder name that will
 * be used as key for the layout. The folder should at least have 2 files:
 *
 * - key.inc
 * - key.tpl.php
 *
 * The css file is optional.
 * - key.css
 *
 * e.g.
 * bartik/ds_layouts/bartik_ds/bartik_ds.inc
 *                            /bartik-ds.tpl.php
 *                            /bartik_ds.css
 *
 * bartik_ds.inc must look like this:
 *

  // Fuction name is ds_LAYOUT_KEY
  function ds_bartik_ds() {
    return array(
      'label' => t('Bartik DS'),
      'regions' => array(
        // The key of this region name is also the variable used in
        // the template to print the content of that region.
        'bartik' => t('Bartik DS'),
      ),
      // Add this if there is a default css file.
      'css' => TRUE,
    );
  }

 */

/**
 * Theme an entity coming from the views entity plugin.
 *
 * @param $entity
 *   The complete entity.
 * @param $view_mode
 *   The name of the view mode.
 */
function ds_views_row_ENTITY_NAME($entity, $view_mode) {
  $nid = $vars['row']->{$vars['field_alias']};
  $node = node_load($nid);
  return drupal_render(node_view($node, $view_mode));
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
