<?php

/**
 * Menu callback: Display Suite extras settings.
 */
function ds_extras_settings($form) {

  $form['switch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom view modes per node'),
  );

  $form['switch']['ds_extras_switch_view_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#description' => t('Toggle this checkbox to enable the "Switch view mode" functionality. With this feature, you can change the view mode on a per node basis. A new tab on the node edit form will appear called "Display settings."'),
    '#default_value' => variable_get('ds_extras_switch_view_mode', FALSE),
  );

  $form['hide_page_title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hide page title'),
  );

  $form['hide_page_title']['ds_extras_hide_page_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#description' => t('Toggle this checkbox to enable the "Hide page title" functionality. With this feature, you can select on the "Manage display" screens if the page title must be hidden or not. <br /><strong>Important: this will only work on the full page of a node.</strong>.'),
    '#default_value' => variable_get('ds_extras_hide_page_title', FALSE),
  );

  if (module_exists('block')) {
    $form['region'] = array(
      '#type' => 'fieldset',
      '#title' => t('Region to block'),
    );

    $form['region']['ds_extras_region_to_block'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable'),
      '#description' => t('Toggle this checkbox to enable the "Region to block" functionality. With this feature, you can create additional regions on the "Manage display" screens which will be exposed as blocks. <br /><strong>Important: this will not work on the default view mode</strong>.'),
      '#default_value' => variable_get('ds_extras_region_to_block', FALSE),
    );
  }

  if (module_exists('fivestar')) {
    $form['fivestar'] = array(
      '#type' => 'fieldset',
      '#title' => t('Fivestar'),
    );

    $form['fivestar']['ds_extras_fivestar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable'),
      '#description' => t('Toggle this checkbox to enable the "Fivestar" functionality. A field called "Fivestar" will be made available on the Manage Display screens.'),
      '#default_value' => variable_get('ds_extras_fivestar', FALSE),
    );
  }

  if (module_exists('views')) {
    $form['views'] = array(
      '#type' => 'fieldset',
      '#title' => t('Views displays'),
    );

    $form['views']['ds_extras_vd'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable'),
      '#description' => t('Toggle this checkbox to enable the "Views displays" functionality. With this feature, you can use Display Suite to rearrange the template variables of any views display. A new menu item called "Views displays" will be available at !url.', array('!url' => l(url('admin/structure/ds'), 'admin/structure/ds', array('absolute' => TRUE)))),
      '#default_value' => variable_get('ds_extras_vd', FALSE),
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'ds_extras_settings_submit';
  return $form;
}

/**
 * Submit callback: Extras settings screen.
 */
function ds_extras_settings_submit($form, &$form_state) {
  // Clear fields cache.
  cache_clear_all('ds_fields', 'cache');
  // Clear module_implements cache and rebuild menu.
  cache_clear_all('entity_info:', 'cache', TRUE);
  cache_clear_all('theme_registry:', 'cache', TRUE);
  cache_clear_all('module_implements', 'cache_bootstrap');
  menu_rebuild();
}
