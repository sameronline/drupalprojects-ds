<?php
// $Id$

/**
 * @file
 * Plugin to render an empty region.
 */

/**
 * Implementation of nd_pluginname_form().
 */
function nd_emptyregionrender_form(&$form, $nd_display_settings) {

  $form['emptyregionrender'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Empty region render'),
    '#description' => t('Toggle regions you want to render when there are no fields in it or when field values are empty'),
  );

  $build_mode = $form['#build_mode'];
  $regions = nd_regions();
  foreach ($regions as $region => $title) {
    if ($region != 'disabled') {
      $form['emptyregionrender']['region-render-'. $region] = array(
        '#title' => $title,
        '#type' => 'checkbox',
        '#default_value' => ds_default_value($nd_display_settings, $build_mode, 'emptyregionrender', 'render', $region, ''),
      );
    }
  }
}

/**
 * Implementation of nd_pluginname_submit().
 */
function nd_emptyregionrender_submit($form, $form_state, &$nd_display_settings) {
  $regions = nd_regions();
  $regions_save = array();
  $build_mode = $form['#build_mode'];
  foreach ($regions as $region => $title) {
    $regions_save['render'][$region] = $form_state['values']['emptyregionrender']['region-render-'. $region];
  }
  $nd_display_settings[$build_mode]['emptyregionrender'] = $regions_save;
}

/**
 * Implementation of nd_pluginname_process().
 */
function nd_emptyregionrender_process(&$vars, &$node_display, $nd_display_settings) {
  foreach ($node_display->all_regions as $region_name => $region_title) {
    $render = ds_default_value($nd_display_settings, $vars['node']->build_mode, 'emptyregionrender', 'render', $region_name, FALSE);
    if ($render == TRUE && !isset($node_display->themed_regions[$region_name])) {
      $node_display->themed_regions[$region_name] = array('content' => '&nbsp;');
      if ($region_name == 'left' || $region_name == 'right') {
        $node_display->region_classes[$region_name] = $region_name;
      }
    }
  }
}
