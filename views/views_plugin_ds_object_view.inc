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
    $options['show_links'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {

    $build_mode_options = array();
    $build_modes = ds_get_build_modes('nd');
    foreach ($build_modes as $build_mode => $value) {
      $build_mode_options[$build_mode] = $value['title'];
    }

    $form['build_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Select the build mode'),
      '#default_value' => $this->options['build_mode'],
      '#options' => $build_mode_options,
    );
    $form['show_links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show links'),
      '#default_value' => $this->options['show_links'],
    );
  }
}
