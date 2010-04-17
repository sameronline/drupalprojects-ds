<?php
// $Id$
/**
 * @file
 * Contains the base row style plugin for Display Sujite.
 */

/**
 *
 * @ingroup views_row_plugins
 */
class views_plugin_ds_fields_view extends views_plugin_row {

  // @todo check views 3 support.
  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {

    $fields = array();
    $display_settings = array();
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      $fields[$field] = array(
        'title' => $handler->ui_name(),
        'status' => 1,
        'type' => 1,
      );

      $display_settings['views']['fields'][$field]['region'] = isset($this->options[$field]['region']) ? $this->options[$field]['region'] : 'disabled';
      $display_settings['views']['fields'][$field]['weight'] = isset($this->options[$field]['weight']) ? $this->options[$field]['weight'] : 0;
      $display_settings['views']['fields'][$field]['css-class'] = isset($this->options[$field]['css-class']) ? $this->options[$field]['css-class'] : '';
    }

    $form['#module'] = 'ds';
    $form['#build_mode'] = 'views';
    $form['#type_name'] = 'views_object';
    $form['#extra'] = array();
    $form['#regions'] = ds_regions();

    module_load_include('inc', 'ds', 'includes/ds.display');
    ds_fields_display_form($form, $display_settings, $fields);

    $path = drupal_get_path('module', 'ds');
    $form['#prefix'] = '<script type="text/javascript" src="'. base_path() . $path .'/js/ds.js?d"></script>' .
        '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $path .'/css/ds.css" />';


    $form['#js']['tableDrag']['fields']['field-weight'][0] = array(
      'target' => 'field-weight',
      'source' => NULL,
      'relationship' => 'sibling',
      'action' => 'order',
      'hidden' => TRUE,
      'limit' => 0,
    );

    $form['#theme'] = 'ds_display_overview_views_form';

    views_ui_standard_form_buttons($form, $form_state, 'views_ui_ds_form');
  }

  /**
   * Make the row_options a bit smaller :)
   */
  function options_submit($form, &$form_state) {
    foreach ($form_state['values']['row_options'] as $field => $value) {
      if ($field != 'buttons') {
        $form_state['values']['row_options'][$field] = array(
          'region' => $value['views']['region'],
          'weight' => $value['ds_weight'],
          'css-class' => $value['views']['css-class'],
        );
        $form_state['values']['row_options']['regions'][$value['views']['region']][$field] = $value['ds_weight'];
      }
    }
    $form_state['values']['row_options']['base_table'] = $this->ds_views_base_table();
  }

  /**
   * Store the base module.
   */
  function ds_views_base_table() {
    $views_base_table = $this->view->base_table;
    $ds_views_fields_modules = array(
      'node' => 'nd',
      'users' => 'ud',
      'comments' => 'cd',
      'heartbeat_activity' => 'hds',
    );

    if (isset($ds_views_fields_modules[$views_base_table])) {
      return $ds_views_fields_modules[$views_base_table];
    }
    return 'ds-views';
  }
}
