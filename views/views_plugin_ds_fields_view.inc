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

      $type = ($handler->definition['handler'] == 'ds_handler_field_fieldgroup') ? DS_FIELD_TYPE_GROUP : DS_FIELD_TYPE_PREPROCESS;

      $fields[$field] = array(
        'title' => $handler->ui_name(),
        'status' => DS_FIELD_STATUS_STATIC,
        'type' => $type,
      );

      // Build the display settings.
      $display_settings['views']['fields'][$field]['region'] = isset($this->options[$field]['region']) ? $this->options[$field]['region'] : 'disabled';
      $display_settings['views']['fields'][$field]['weight'] = isset($this->options[$field]['weight']) ? $this->options[$field]['weight'] : 0;
      $display_settings['views']['fields'][$field]['css-class'] = isset($this->options[$field]['css-class']) ? $this->options[$field]['css-class'] : '';
    }

    $form['#module'] = 'ds';
    $form['#build_mode'] = 'views';
    $form['#type_name'] = 'views_object';
    $form['#extra'] = array();
    $form['#regions'] = ds_regions();

    // Load the drag and drop form.
    module_load_include('inc', 'ds', 'includes/ds.display');
    ds_fields_display_form($form, $display_settings, $fields);

    // Add our javascript and css.
    $path = drupal_get_path('module', 'ds');
    $form['#prefix'] = '<script type="text/javascript" src="'. base_path() . $path .'/js/ds.js?d"></script>' .
        '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . $path .'/css/ds.css" />';

    // Special js stuff for drag and drop.
    $form['#js']['tableDrag']['fields']['field-weight'][0] = array(
      'target' => 'field-weight',
      'source' => NULL,
      'relationship' => 'sibling',
      'action' => 'order',
      'hidden' => TRUE,
      'limit' => 0,
    );
    /*$form['#js']['tableDrag']['fields']['ds-field-id'][0] = array(
      'target' => 'parent',
      'source' => 'ds-field-id',
      'group' => 'ds-parent-id',
      'subgroup' => 'ds-parent-id',
      'relationship' => 'parent',
      'action' => 'match',
      'hidden' => TRUE,
      'limit' => 1,
    );*/
  //drupal_add_tabledrag('fields', 'match', 'parent', 'ds-parent-id', 'ds-parent-id', 'ds-field-id', FALSE, 1);

    // We're using a different theme.
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
    $form_state['values']['row_options']['module'] = $this->ds_views_base_table('module');
    $form_state['values']['row_options']['object_type'] = $this->ds_views_base_table('object_type');
  }

  /**
   * Store the base module.
   */
  function ds_views_base_table($key) {
    $views_base_table = $this->view->base_table;
    $ds_views_fields_modules = array(
      'node' => array(
        'module' => 'nd',
        'object_type' => 'node',
      ),
      'users' => array(
        'module' => 'ud',
        'object_type' => 'user'
      ),
      'comments' => array(
        'module' => 'cd',
        'object_type' => 'comment',
      ),
      'heartbeat_activity' => array(
        'module' => 'hds',
        'object_type' => 'heartbeatactivity',
      ),
    );

    if (isset($ds_views_fields_modules[$views_base_table])) {
      return $ds_views_fields_modules[$views_base_table][$key];
    }
    return 'ds-views';
  }

  /**
   * Render the fields.
   *
   * @param array $fields Collection of fields for display.
   */
  function ds_fields_render($fields) {

    $object = new stdClass;
    $object->type = 'all';
    $object->ds_groups = array();
    $object->build_mode = 'ds_views';
    $object_type = $this->view->style_plugin->row_plugin->options['object_type'];
    $vars[$object_type] = $object_type;
    $vars['regions'] = $this->view->style_plugin->row_plugin->options['regions'];
    $module = $this->view->style_plugin->row_plugin->options['module'];

    // Loop through views fields, see if they are a field or a group.
    foreach ($fields as $key => $field) {

      // Field content.
      $vars[$key] = $field->content;

      // Class.
      $class = $field->class;
      if (!empty($this->view->style_plugin->row_plugin->options[$key]['css-class'])) {
        $class .= ' '. $this->view->style_plugin->row_plugin->options[$key]['css-class'];
      }

      // Build field key in ds_fields.
      $object->ds_fields[$key] = array(
        'type' => 'ds',
        'title' => $field->label,
        'labelformat' => 'inline', // @todo - bring back inline labels on the form.
        'class' => $class,
      );

      // Add key to preprocess fields.
      $object->preprocess_fields[$key] = DS_FIELD_TYPE_PREPROCESS;
    }

    // Return the row.
    return ds_render_content($object, $module, $vars);
  }

}
