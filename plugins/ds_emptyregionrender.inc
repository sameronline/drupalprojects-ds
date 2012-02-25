<?php

/**
 * @file
 * DS_EmptyRegionRender: Plugin to render an empty region.
 */

class ds_emptyregionrender {

  /**
   * Constructor().
   */
  function __construct() {

  }

  /**
   * plugin_form().
   */
  public function plugin_form(&$form, $display_settings) {

    $form['emptyregionrender'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Toggle regions you want to render when there are no fields in it or when field values are empty.'),
    );

    $build_mode = $form['#build_mode'];
    $regions = ds_regions();
    foreach ($regions as $region => $title) {
      if ($region != 'disabled') {
        $form['emptyregionrender']['region-render-'. $region] = array(
          '#title' => $title,
          '#type' => 'checkbox',
          '#default_value' => ds_default_value($display_settings, 'emptyregionrender', 'render', $region, ''),
        );
      }
    }
  }

  /**
   * plugin_form_submit().
   */
  public function plugin_form_submit($form, $form_state, &$display_settings) {

    $regions = ds_regions();
    $regions_save = array();
    $build_mode = $form['#build_mode'];

    foreach ($regions as $region => $title) {
      if (isset($form_state['values']['emptyregionrender']['region-render-'. $region]))
      $regions_save['render'][$region] = $form_state['values']['emptyregionrender']['region-render-'. $region];
    }

    $display_settings[$build_mode]['emptyregionrender'] = $regions_save;
  }

  /**
   * execute().
   */
  public function execute($vars, &$display, $display_settings, $object_type, $module) {

    foreach ($display->themed_regions as $region_name => $region_title) {

      $render = ds_default_value($display_settings, 'emptyregionrender', 'render', $region_name, FALSE);
      if ($render == TRUE) {
        $display->themed_regions[$region_name]['hidden'] = FALSE;
      }
    }
  }
}