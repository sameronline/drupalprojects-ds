<?php
// $Id$

/**
 * @file
 * Contains the ds views node style plugin.
 */

/**
 * Plugin which sets a build mode on the resulting object.
 */
class views_plugin_ds_object_view extends views_plugin_row {

  // Views 3 support: set base_table and base_field.
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
    if (version_compare(views_api_version(), '2.0') == 1) {
      $this->base_table = $view->base_table;
      $this->base_field = $this->ds_views_3_support();
    }
  }

  // Return base_field based on base_table. It might not be
  // the cleanest solution, it's the fastest though. No
  // need to a lot of mumbo jumbo here.
  function ds_views_3_support() {
    $base_table_fields = array(
      'node' => 'nid',
      'users' => 'uid',
      'comments' => 'cid',
      'apachesolr' => 'nid',
    );
    return $base_table_fields[$this->base_table];
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['build_mode'] = array('default' => 'teaser');
    $options['advanced'] = FALSE;
    if (isset($this->view->base_table)) {
      $options['relationship'] = array('default' => 'none');
    }
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $views_base_table = $this->view->base_table;

    if (isset($this->view->base_table)) {
      $view = &$form_state['view'];

      // A whole bunch of code to figure out what relationships are valid for
      // this item.
      $relationships = $view->display_handler->get_option('relationships');
      $relationship_options = array();

      foreach ($relationships as $relationship) {
        $relationship_handler = views_get_handler($relationship['table'], $relationship['field'], 'relationship');

        // If this relationship is valid for this type, add it to the list.
        $data = views_fetch_data($relationship['table']);
        $base = $data[$relationship['field']]['relationship']['base'];
        if ($base == $this->view->base_table) {
          $relationship_handler->init($view, $relationship);
          $relationship_options[$relationship['id']] = $relationship_handler->label();
        }
      }

      if (!empty($relationship_options)) {
        $relationship_options = array_merge(array('none' => t('Do not use a relationship')), $relationship_options);
        $rel = empty($this->options['relationship']) ? 'none' : $this->options['relationship'];
        if (empty($relationship_options[$rel])) {
          // Pick the first relationship.
          $rel = key($relationship_options);
        }

        $form['relationship'] = array(
          '#type' => 'select',
          '#title' => t('Relationship'),
          '#options' => $relationship_options,
          '#default_value' => $rel,
        );
      }
      else {
        $form['relationship'] = array(
          '#type' => 'value',
          '#value' => 'none',
        );
      }
    }

    $build_mode_options = array();
    $build_modes = ds_get_build_modes();
    foreach ($build_modes as $module => $modes) {
      $function = $module .'_ds_api';
      $api_info = $function();
      // $api_info['views_base'] can either be a string or an array.
      if ((is_array($api_info['views_base']) && in_array($views_base_table, $api_info['views_base'])) || (is_string($api_info['views_base']) && $api_info['views_base'] == $views_base_table)) {
        foreach ($modes as $key => $title) {
          $build_mode_options[$key] = $title['title'];
        }
      }
    }

    $form['build_mode'] = array(
      '#type' => 'select',
      '#title' => t('Select the build mode'),
      '#default_value' => $this->options['build_mode'],
      '#options' => $build_mode_options,
      '#description' => t('Make sure you select a build mode which is compatible with the base table of this view which is %base_table. <br />Also note that if you excluded build modes, you will still see them listed here as it\'s possible to list different types in a view.', array('%base_table' => $this->view->base_table))
    );

    $form['advanced'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the advanced build mode selector'),
      '#description' => t('This gives you the opportunity to have more control in a list, eg the first build mode on the object is A and the rest B.<br /> There is no UI for this, you need to create a function named like this: <strong><em>ds_views_row_adv_@VIEWSNAME(&$variables, $build_mode)</em></strong>.<br />See <a href="http://drupal.org/node/697320#ds_views_row_adv_VIEWSNAME">http://drupal.org/node/697320#ds_views_row_adv_VIEWSNAME</a> for an example.', array('@VIEWSNAME' => $this->view->name)),
      '#default_value' => $this->options['advanced'],
    );
  }

  function query() {
    if (isset($this->view->base_table) && isset($this->options['relationship']) && isset($this->view->relationship[$this->options['relationship']])) {
      $relationship = $this->view->relationship[$this->options['relationship']];
      $this->field_alias = $this->view->query->add_field($relationship->alias, $this->view->base_field);
    }
    else {
      $this->field_alias = $this->view->base_field;
    }
  }
}
