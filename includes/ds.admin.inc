<?php
// $Id$

/**
 * @file
 * General file for administrative functions.
 */

function ds_general_page() {

  // Child pages.
  module_load_include('inc', 'system', 'system.admin');
  $childs = system_admin_menu_block_page();
  $fieldset = array();
  $fieldset['#title'] = 'General';
  $fieldset['#children'] = $childs;
  $output = theme('fieldset', $fieldset);

  // General settings
  $output .= drupal_get_form('ds_general_settings');

  return $output;
}

function ds_general_settings($form_state) {
  $form = array();

  $form['regions_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regions css'),
  );
  $form['regions_css']['ds_regions_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load region css.'),
    '#description' => t('Load the default region css that comes with this module. For inspiration, look into css/regions.css. Note, do not make any changes in that file as module upgrades will overwrite your changes.'),
    '#default_value' => variable_get('ds_regions_css', TRUE),
  );

  // One button to save them all.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Settings submit.
 */
function ds_general_settings_submit($form, &$form_state) {
  variable_set('ds_regions_css', $form_state['values']['ds_regions_css']);
  drupal_set_message(t('The configuration options have been saved.'));
}

