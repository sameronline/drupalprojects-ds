<?php

/**
 * @file
 * Administrative functions for Display Suite search.
 */

/**
 * Menu callback: Display Suite search settings.
 */
function ds_search_settings($form, $form_state) {
  $config = config('ds.search');

  $form['general'] = array(
    '#type' => 'details',
    '#title' => t('General'),
  );

  $engine_options = array();
  foreach (module_implements('search_info') as $module) {
    if ($module != 'ds_search' && $module != 'user') {
      $engine_options[$module] = drupal_ucfirst(str_replace('_', ' ', $module));
    }
  }

  $form['general']['search_type'] = array(
    '#type' => 'select',
    '#title' => t('Search engine'),
    '#description' => t('Select the search engine as the query backend.'),
    '#options' => $engine_options,
    '#default_value' => $config->get('search_type'),
  );

  $form['general']['search_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Search path'),
    '#field_prefix' => 'search/',
    '#description' => t('Make sure you don\'t override an existing search path.'),
    '#default_value' => $config->get('search_path'),
    '#required' => TRUE,
  );

  $entity_view_modes = entity_get_view_modes('node');
  foreach ($entity_view_modes as $key => $view_mode) {
    $view_mode_options[$key] = $view_mode['label'];
  }
  $form['general']['view_mode'] = array(
    '#type' => 'select',
    '#title' => t('View mode'),
    '#description' => 'Select another view mode in favor of the default search view mode.',
    '#default_value' => $config->get('view_mode'),
    '#options' => $view_mode_options,
  );

  $form['general']['search_variables'] = array(
    '#type' => 'radios',
    '#options' => array(
      'none' => t('None'),
      'search_totals' => t('Total results'),
      'search_totals_plus_keywords' => t('Total results + keywords'),
      'search_totals_from_to_end' => t('Totals + start to end')
    ),
    '#title' => t('Extra variables'),
    '#description' => t('Choose an extra variable to display on the results screen.'),
    '#default_value' => $config->get('search_variables'),
  );

  $form['general']['show_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show title'),
    '#description' => t('Display the "Search results" title.'),
    '#default_value' => $config->get('show_title'),
  );

  $form['general']['search_language'] = array(
    '#type' => 'checkbox',
    '#title' => t('Language'),
    '#description' => t('Search in the language the site is currently in. Enable this if you have at least 2 languages.'),
    '#default_value' => $config->get('search_language'),
  );

  $form['general']['highlight'] = array(
    '#type' => 'checkbox',
    '#title' => t('Highlight search word'),
    '#description' => t('Use jQuery to highlight the word in the results.'),
    '#default_value' => $config->get('highlight'),
  );

  $form['general']['highlight_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('HTML selector'),
    '#description' => t('Enter the css selector, if not sure, leave this by default.'),
    '#default_value' => $config->get('highlight_selector'),
    '#states' => array(
      'visible' => array(
        'input[name="highlight"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['node'] = array(
    '#type' => 'details',
    '#title' => t('Drupal core'),
    '#states' => array(
      'visible' => array(
        'select[name="search_type"]' => array('value' => 'node'),
      ),
    ),
  );

  $form['node']['node_form_alter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Advanced'),
    '#description' => t('Enable the advanced search form.'),
    '#default_value' => $config->get('node_form_alter'),
  );

  $form['node']['search_node_limit'] = array(
    '#type' => 'textfield',
    '#default_value' => config('ds.search')->get('search_node_limit'),
    '#title' => t('Node search limit'),
    '#description' => t('The number of items to display per page. Enter 0 for no limit.'),
  );

  $active = config('search.settings')->get('active_modules');
  if (isset($active['user']) && $active['user'] !== 0) {
    $form['user'] = array(
      '#type' => 'details',
      '#title' => t('User search'),
    );

    $form['user']['user_override_search_page'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override user search page'),
      '#description' => t('Toggle this checkbox if you want Display Suite to render user search results.'),
      '#default_value' => $config->get('user_override_search_page'),
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'ds_search_settings_submit';
  return $form;
}

/**
 * Submit callback: Search settings submit.
 */
function ds_search_settings_submit($form, $form_state) {

  config('ds.search')
    ->set('search_type', $form_state['values']['search_type'])
    ->set('view_mode', $form_state['values']['view_mode'])
    ->set('search_path', $form_state['values']['search_path'])
    ->set('search_variables', $form_state['values']['search_variables'])
    ->set('show_title', $form_state['values']['show_title'])
    ->set('search_language', $form_state['values']['search_language'])
    ->set('highlight', $form_state['values']['highlight'])
    ->set('highlight_selector', $form_state['values']['highlight_selector'])
    ->set('node_form_alter', $form_state['values']['node_form_alter'])
    ->set('search_node_limit', $form_state['values']['search_node_limit'])
    ->set('user_override_search_page', $form_state['values']['user_override_search_page'])
    ->save();


  // Rebuild the menu.
  menu_router_rebuild();
}
