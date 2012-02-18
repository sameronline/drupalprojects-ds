<?php

/**
 * @file
 * dsRegionToBlock: Plugin to make a region available as a block.
 */

$plugin = array(
  'name' => t('Use regions as blocks'),
  'class' => 'dsRegionToBlock',
  'description' => t('Render a display suite region into a theme region. That region will be made available as a block.'),
  'target' => 'all',
);

class dsRegionToBlock {

  /**
   * Constructor().
   */
  function __construct() {

  }

  /**
   * plugin_form().
   */
  public function plugin_form(&$form, $display_settings) {

    $form['regiontoblock'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Toggle regions you want to make available as a block.'),
    );

    $build_mode = $form['#build_mode'];
    $regions = ds_get_regions($display_settings['layout']);
    foreach ($regions as $key => $region) {
      if ($key != DS_DISABLED_REGION) {
        $default_value = ds_default_value($display_settings, 'regiontoblock', 'block', $key, '');
        $form['regiontoblock']['region-block-'. $key] = array(
          '#title' => $region['#name'],
          '#type' => 'checkbox',
          '#default_value' => $default_value,
        );
        if ($default_value == TRUE) {
          $form['#regions'][$key] .= ' - '. t('assigned to block');
        }
      }
    }
  }

  /**
   * plugin_form_submit().
   */
  public function plugin_form_submit($form, $form_state, &$display_settings) {

    $regions = ds_get_regions($display_settings['layout']);
    $regions_save = array();
    $build_mode = $form['#build_mode'];

    foreach ($regions as $key => $region) {
      if (isset($form_state['values']['regiontoblock']['region-block-'. $key]))
      $regions_save['block'][$key] = $form_state['values']['regiontoblock']['region-block-'. $key];
    }

    $display_settings[$build_mode]['regiontoblock'] = $regions_save;
  }

  /**
   * plugin block listing.
   *
   * Make sure you create a unique name for your $block_key.
   * You should return info & plugin so ds_block knows
   * which block to load. You can add other info, which
   * you'll receive back in block_list.
   */
  public function block_list($module, &$blocks, $data) {
    $api_info = ds_api_info($module);
    $types = $api_info['types']();
    if (!empty($types)) {
      foreach ($types as $type_key => $object) {
        $display_settings = array();
        $records = db_query("SELECT * FROM {ds_settings} WHERE module = '%s' AND type = '%s'", $module, $type_key);
        while ($row = db_fetch_object($records)) {
          $display_settings[$row->build_mode] = unserialize($row->settings);
        }
        foreach ($display_settings as $build_mode => $settings) {

          if (isset($settings['regiontoblock']['block'])) {
            foreach ($settings['regiontoblock']['block'] as $region => $value) {
              if ($value) {
                $block_key = 'ds_regiontoblock_'. $module .'_'. $object->type .'_'. $build_mode .'_'. $region;
                $block_info = $api_info['title'] .': '. $object->name .', '. $build_mode .', '.$region;
                $blocks[] = array(
                  'info' => $block_info,
                  'key' => $block_key,
                  'data' => $data,
                );
              }
            }
          }
        }
      }
    }
  }

  /**
   * plugin block view
   */
  public function block_view($block) {
    $content = array();
    $data = ds_static_variables($block['key']);
    if (!empty($data)) {
      $content['subject'] = $block['info'];
      $content['content'] = theme('ds_objects', $data, array('context' => 'blocktoregion', 'block' => $block));
    }
    return $content;
  }

  /**
   * execute().
   */
  public function execute($vars, &$display, $display_settings, $object_type, $module) {
    $object = $vars[$object_type];

    foreach ($display->regions as $region_name => $region) {
      if ($display->regionIsActive($region_name)) {
        $region_to_block = ds_default_value($display_settings, 'regiontoblock', 'block', $region_name, FALSE);

        if (isset($region_to_block) && $region_to_block == TRUE) {
          $key = 'ds_regiontoblock_'. $module .'_'. $object_type .'_'. $display->build_mode .'_'. $region_name;
          $data = new stdClass;
          $data->content = $display->regions[$region_name]['#field_content'];
          $data->object = $vars[$object_type];
          ds_static_variables($key, $data);
          $display->regionHide($region_name);
        }
      }
    }
  }
}
