<?php
// $Id$

/**
 * @file
 * Manage groups.
 */

/**
 * Groups overview.
 */
function ds_groups($module) {
  $output = '';
  $action = arg(4);
  $group = arg(5);
  $groups = variable_get($module .'_groups', array());
  $api_info = ds_api_info($module);

  // Delete form.
  if (in_array($group, array_keys($groups)) && $action == 'delete') {
    $output .= drupal_get_form('ds_group_delete_form', $module, $group);
  }
  // Edit form.
  elseif (in_array($group, array_keys($groups)) && $action == 'edit') {
    $output .= drupal_get_form('ds_group_form', $module, $group);
  }
  // Custom build modes overview form.
  else {
    if (empty($groups)) {
      $output .= '<p>'. t('You have not defined any custom groups.') .'</p>';
    }
    else {
      $output .= '<p>'. t('This is a list of custom groups defined via the UI.') .'</p>';

      $header = array(
        t('Name'), t('Key'), t('Operations'),
      );

      $rows = array();
      foreach ($groups as $key => $name) {
        $row = array();
        $row[] = check_plain($name);
        $row[] = $key;
        $row[] = l(t('Edit'), 'admin/ds/'. $module .'/groups/edit/'. $key) .' - '. l(t('Delete'), 'admin/ds/'. $module .'/groups/delete/'. $key);
        $rows[] = $row;
      }

      $output .= theme('table', $header, $rows);
    }

    // New group form.
    $output .= drupal_get_form('ds_group_form', $module);
  }


  return $output;
}

/**
 * Group form.
 */
function ds_group_form($form_state, $module, $group = '') {
  $form = array();

  if (!empty($group)) {
    $groups = variable_get($module .'_groups', array());
    $title = $groups[$group];
  }
  else {
    $title = '';
  }

  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($build_mode)) ? t('Add new group'): t('Update group'),
    '#description' => '<strong>'. t('There is no validation against existing groups, eg coming from CCK, so make sure you create unique keys for groups!') .'</strong>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['identity']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'Key name',
    '#description' => t('The machine-readable name of this group.'),
    '#required' => TRUE,
  );
  if (!empty($build_mode)) {
    $form['identity']['key']['#disabled'] = TRUE;
    $form['identity']['key']['#value'] = $build_mode;
    $form['identity']['key']['#description'] = t('The machine-readable name of this group. Note: you can not edit this key.');
  }
  $form['identity']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Group label',
    '#description' => t('The name of this label which will be used as default label/title on the display fields page.'),
    '#required' => TRUE,
    '#default_value' => $title,
  );
  $form['identity']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save group'),
    '#submit' => array('ds_group_form_submit'),
  );

  $form['#module'] = $module;
  $form['#form_type'] = (empty($group)) ? 'insert' : 'update';

  return $form;
}

/**
 * Validate new custom group submission.
 */
/*function ds_group_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $existing = array_keys(ds_get_build_modes($form['#module']));
    if (in_array($form_state['values']['key'], $existing)) {
      form_set_error('key', t('This build mode already exists.'));
    }

    if (!preg_match('!^[a-z_]+$!', $form_state['values']['key'])) {
      form_set_error('key', t('The machine-readable name must contain only lowercase letters and underscores.'));
    }
  }
}*/

/**
 * Save new build mode.
 */
/*function ds_build_mode_form_submit($form, &$form_state) {
  $build_modes = variable_get($form['#module'] .'_build_modes', array());
  $build_modes[$form_state['values']['key']] = $form_state['values']['name'];
  variable_set($form['#module'] .'_build_modes', $build_modes);

  drupal_set_message(t('Build mode %build_mode has been saved.', array('%build_mode' => $build_modes[$form_state['values']['key']])));
  $form_state['redirect'] = 'admin/ds/'. $form['#module'] .'/buildmodes';
  menu_rebuild();
}*/

/**
 * Build mode delete form.
 */
/*function ds_build_mode_delete_form($form_state, $module, $build_mode) {
  $form = array();
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to delete the build mode type %build_mode ?', array('%build_mode' => $build_mode)) .'</p>',
  );
  $form['display'] = array(
    '#type' => 'value',
    '#value' => $build_mode,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/ds/'. $module .'/buildmodes'),
  );

  $form['#module'] = $module;
  return $form;
}*/

/**
 * Delete build mode.
 */
/*function ds_build_mode_delete_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $build_modes = variable_get($module .'_build_modes', array());
  unset($build_modes[$form_state['values']['display']]);
  variable_set($module .'_build_modes', $build_modes);

  drupal_set_message(t('Custom build mode %build_mode has been removed.', array('%build_mode' => $form_state['values']['display'])));
  $form_state['redirect'] = 'admin/ds/'. $module .'/buildmodes';
  menu_rebuild();
}*/


