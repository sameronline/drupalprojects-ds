<?php
// $Id$

/**
 * @file
 * Contains the nd view style plugin.
 */

/**
 * Plugin which performs a object on the resulting object based on ds build modes.
 */
class views_plugin_ds_object_view extends views_plugin_row {

  function option_definition() {
    $options = parent::option_definition();
    $options['build_mode'] = array('default' => 'teaser');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $build_mode_options = array();
    $build_modes = ds_get_build_modes();
    foreach ($build_modes as $module => $modes) {
      foreach ($modes as $key => $title) {
        $build_mode_options[strtoupper($module)][$key] = $title['title'];
      }
    }
    $form['build_mode'] = array(
      '#type' => 'select',
      '#title' => t('Select the build mode'),
      '#default_value' => $this->options['build_mode'],
      '#options' => $build_mode_options,
      '#description' => t('Make sure you select a build mode which is compatible with the base table of this view which is %base_table.', array('%base_table' => $this->view->base_table))
    );
  }
}
