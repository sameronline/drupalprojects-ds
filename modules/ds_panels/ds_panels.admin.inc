<?php

/**
 * @file
 * Administrative functions for DS panels.
 */

/**
 * Manage display screen.
 */
function ds_panels_field_ui($entity_type, $bundle, $view_mode) {

  $build = array();
  $panel_layout = '';

  // Extra the bundle name.
  $bundle = field_extract_bundle($entity_type, $bundle);

  // Load ds.
  module_load_include('inc', 'ds', 'ds.field_ui');

  // Get the ds layout.
  $ds_layout = ds_get_layout($entity_type, $bundle, $view_mode);

  // Check if the layout is added as an extra argument in the URI.
  // Depending on the outcome, the selection tab will display
  // links, a form or the layout edit form.
  $layout_tab_first = FALSE;
  $menu = menu_get_item();
  if (!empty($menu['theme_arguments'])) {
    $layout_tab_first = TRUE;
    $panel_layout = array_shift($menu['theme_arguments']);
  }

  // Get the pieces of this page.
  $layout = drupal_get_form('ds_panels_layout', $entity_type, $bundle, $view_mode, $ds_layout, $panel_layout, $layout_tab_first);
  $content = ds_panels_content($entity_type, $bundle, $view_mode, $ds_layout, $panel_layout);

  // Move all this stuff into nice vertical tabs.
  $build['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#theme_wrappers' => array('vertical_tabs'),
  );

  $build['tabs']['ds_panels_content'] = array(
    '#type' => 'fieldset',
    '#group' => 'tabs',
    '#title' => t('Content'),
    '#weight' => 0,
  );
  $build['tabs']['ds_panels_content']['content'] = array(
    '#markup' => drupal_render($content),
  );
  $build['tabs']['ds_panels_layout'] = array(
    '#type' => 'fieldset',
    '#group' => 'tabs',
    '#title' => t('Layout'),
    '#weight' => ($layout_tab_first || empty($ds_layout)) ? -1 : 1,
  );
  $build['tabs']['ds_panels_layout']['content'] = array(
    '#markup' => drupal_render($layout)
  );

  // Add the Custom display settings form.
  if ($view_mode == 'default') {
    //$layout = drupal_get_form('ds_panels_layout', $entity_type, $bundle, $view_mode);
  }

  return $build;
}

/**
 * Add the Panels layout/content manager.
 */
function ds_panels_layout($form, $form_state, $entity_type, $bundle, $view_mode, $ds_layout, $panel_layout, $layout_tab_first) {

  $form['#entity_type'] = $entity_type;
  $form['#bundle'] = $bundle;
  $form['#view_mode'] = $view_mode;
  $form['#export_id'] = $entity_type . '|' . $bundle . '|' . $view_mode;
  $form['#ds_panels_update'] = !empty($ds_layout) ? TRUE : FALSE;

  // Include panel functions.
  ctools_include('plugins', 'panels');
  ctools_include('common', 'panels');

  // When panel_layout is empty, render the layout selector with links.
  if (empty($ds_layout) && empty($panel_layout)) {
    $layout_tab_first = TRUE;
    $layouts = panels_common_get_allowed_layouts('ds_panels');
    $form['layout'] = array(
      '#markup' => panels_common_print_layout_links($layouts, $_GET['q'], array('query' => $_GET))
    );
    return $form;
  }

  // If panel_layout is not empty and the layout key is in the URI
  // show the layout change form of panels.
  if (!empty($ds_layout) && !empty($panel_layout)) {
    // @todo next does not work here yet, will probably have to redirect.
    $display = panels_load_display($ds_layout['settings']['did']);
    $display->context = ds_get_entity_context($entity_type);
    $form['layout'] = array(
      '#markup' => drupal_render(panels_edit_layout($display, t('Save'), $_GET['q'], 'ds_panels'))
    );
    return $form;
  }

  // Settings form.
  if (!empty($ds_layout)) {
    $panel_layout = $ds_layout['settings']['layout'];
  }

  $form['ds_panels_layout']['layout'] = array(
    '#type' => 'value',
    '#value' => $panel_layout,
  );

  $form['ds_panels_layout']['css_id'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS ID'),
    '#size' => 30,
    '#description' => t('An ID that can be used by CSS to style the panel.'),
    '#default_value' => isset($layout['settings']['css_id']) ? $layout['settings']['css_id'] : '',
  );

  // Support for different rendering pipelines
  $pipelines = panels_get_renderer_pipelines();

  $options = array();
  foreach ($pipelines as $name => $pipeline) {
    $options[$name] = check_plain($pipeline->admin_title) . '<div class="description">' . check_plain($pipeline->admin_description) . '</div>';
  }

  $form['ds_panels_layout']['pipeline'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#title' => t('Renderer'),
    '#default_value' => isset($layout['settings']['css_id']) ? $layout['settings']['css_id'] : 'standard',
  );

  $form['ds_panels_layout']['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save layout'),
    '#submit' => array('ds_panels_layout_save'),
  );

  if (isset($ds_layout['settings']['did'])) {
    $form['#did'] = $ds_layout['settings']['did'];
    $form['ds_panels_layout']['actions']['layout_change'] = array(
      '#type' => 'submit',
      '#value' => t('Change layout'),
      '#submit' => array('ds_panels_layout_change'),
    );
    $form['ds_panels_layout']['actions']['layout_remove'] = array(
      '#type' => 'submit',
      '#value' => t('Delete layout'),
      '#submit' => array('ds_panels_layout_remove'),
    );
  }

  // Check if we need to strip out the panel layout key.
  $form['#strip_off'] = $layout_tab_first;
  $form['#attached']['js'][] = drupal_get_path('module', 'ds_panels') . '/ds_panels.js';

  return $form;
}

/**
 * Return the panels content display.
 */
function ds_panels_content($entity_type, $bundle, $view_mode, $ds_layout, $panel_layout) {

  if (empty($ds_layout)) {
    return array('#markup' => t('Select a layout first.'));
  }

  $did = $ds_layout['settings']['did'];
  ctools_include('common', 'panels');
  $display = panels_load_display($did);
  $display->context = ds_get_entity_context($entity_type);
  $content_types = panels_common_get_allowed_types('panels_ds', $display->context);
  $panel_display = panels_edit($display, $_GET['q'], $content_types);

  // Remove the preview and render the panels content form.
  $panel_display['preview']['#access'] = FALSE;
  return $panel_display;
}

/**
 * Submit callback: Save a layout.
 */
function ds_panels_layout_save($form, &$form_state) {

  // Setup some variables.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Prepare settings.
  $settings = array(
    'css_id' => $form_state['values']['css_id'],
    'layout' => $form_state['values']['layout'],
    'pipeline' => $form_state['values']['pipeline'],
    'ds_panels' => TRUE,
  );

  // Panels display.
  if (!$form['#ds_panels_update']) {
    $display = panels_new_display();
    $display->layout = $settings['layout'];
    panels_save_display($display);
    $settings['did'] = $display->did;
  }
  else {
    $settings['did'] = $form['#did'];
  }

  // Remove old record.
  db_delete('ds_layout_settings')
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('view_mode', $view_mode)
    ->execute();

  // Save new record.
  $record = new stdClass();
  $record->id = $form['#export_id'];
  $record->entity_type = $entity_type;
  $record->bundle = $bundle;
  $record->view_mode = $view_mode;
  $record->layout = $settings['layout'];
  $record->settings = $settings;
  drupal_write_record('ds_layout_settings', $record);

  // Clear entity info cache.
  cache_clear_all('entity_info', 'cache', TRUE);

  // Show message.
  drupal_set_message(t('The layout has been saved.'));

  // Strip off layout.
  if ($form['#strip_off']) {
    $form_state['redirect'] = str_replace('/' . $settings['layout'], '', $_GET['q']);
  }
}

/**
 * Submit callback: Redirect to change the layout.
 */
function ds_panels_layout_change($form, &$form_state) {
  $form_state['redirect'] = $_GET['q'] . '/' . $form_state['values']['layout'];
}

/**
 * Submit callback: remove a layout.
 */
function ds_panels_layout_remove($form, $form_state) {

  // Setup some variables.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Delete panels display.
  panels_delete_display($form['#did']);

  // Remove old record.
  db_delete('ds_layout_settings')
    ->condition('entity_type', $entity_type)
    ->condition('bundle', $bundle)
    ->condition('view_mode', $view_mode)
    ->execute();

  // Clear entity info cache.
  cache_clear_all('entity_info', 'cache', TRUE);

  // Show message.
  drupal_set_message(t('The layout has been removed.'));
}
