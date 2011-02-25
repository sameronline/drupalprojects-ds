<?php

/**
 * @file
 * Manage build modes.
 */

/**
 * Build modes overview.
 */
function ds_build_modes($module) {
  $output = '';
  $action = arg(5);
  $build_mode = arg(6);
  $build_modes = variable_get($module .'_build_modes', array());

  // Delete form.
  if (in_array($build_mode, array_keys($build_modes)) && $action == 'delete') {
    $output .= drupal_get_form('ds_build_mode_delete_form', $module, $build_mode);
  }
  // Edit form.
  elseif (in_array($build_mode, array_keys($build_modes)) && $action == 'edit') {
    $output .= drupal_get_form('ds_build_mode_form', $module, $build_mode);
  }
  // Custom build modes overview form.
  else {
    if (empty($build_modes)) {
      $output .= '<p>'. t('You have not defined any custom build modes.') .'</p>';
    }
    else {
      $header = array(
        t('Name'), t('Key'), t('Operations'),
      );

      $rows = array();
      foreach ($build_modes as $key => $name) {
        $row = array();
        $row[] = check_plain($name);
        $row[] = $key;
        $row[] = l(t('Edit'), 'admin/content/types/nd/buildmodes/edit/'. $key) .' - '. l(t('Delete'), 'admin/content/types/nd/buildmodes/delete/'. $key);
        $rows[] = $row;
      }

      $output .= theme('table', $header, $rows);
    }

    // New build mode form.
    $output .= drupal_get_form('ds_build_mode_form', $module);

    // Build modes matrix form.
    $output .= drupal_get_form('ds_buildmodes_matrix_form', $module);
  }


  return $output;
}

/**
 * Build modes matrix form.
 */
function ds_buildmodes_matrix_form($form_state, $module) {

  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds_buildmodes.js');

  // Exclude build mode matrix.
  $exclude_build_modes = variable_get($module .'_buildmodes_exclude', array());
  $build_modes = array('nd_block_all' => array('title' => t('Toggle all'))) + ds_get_build_modes($module);
  $content_types = node_get_types();

  $form['#build_modes'] = $build_modes;
  $form['#content_types'] = $content_types;

  foreach ($content_types as $ckey => $type) {

    $row_exluded = variable_get('nd_contenttype_'. $ckey, FALSE);
    foreach ($build_modes as $bkey => $mode) {

      // We create 2 form keys here. The first one is the checkbox,
      // the second is a hidden value to store the default value in.
      $form['exclude'][$ckey][$ckey .'-'. $bkey] = array(
        '#type' => 'checkbox',
        '#attributes' => array('class' => 'checkbox-instance')
      );
      $form['exclude'][$ckey][$ckey .'-'. $bkey .'-disabled'] = array(
        '#type' => 'value',
        '#value' => $exclude_build_modes[$ckey][$bkey],
      );

      if ($bkey == 'nd_block_all') {
        $form['exclude'][$ckey][$ckey .'-'. $bkey]['#attributes']['class'] .= ' block-all';
        $form['exclude'][$ckey][$ckey .'-'. $bkey]['#default_value'] = $row_exluded;
      }
      else {
        $form['exclude'][$ckey][$ckey .'-'. $bkey]['#default_value'] = $exclude_build_modes[$ckey][$bkey];
        if ($row_exluded) {
          $form['exclude'][$ckey][$ckey .'-'. $bkey]['#disabled'] = 'disabled';
        }
      }
    }
  }

  $form['exclude']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('nd_build_modes_matrix_submit'),
  );

  return $form;
}

/**
 * Theme build modes matrix form.
 */
function theme_ds_buildmodes_matrix_form($form) {
  $output = '';

  // Header.
  $header = array(0 => '');
  foreach ($form['#build_modes'] as $key => $mode) {
    $header[] = $mode['title'];
  }

  // Checkboxes.
  $rows = array();
  foreach ($form['#content_types'] as $key => $type) {
    $row = array();
    $row[] = array('data' => $type->name);
    foreach ($form['#build_modes'] as $bkey => $mode) {
      $row[] = array('data' => drupal_render($form['exclude'][$key][$key .'-'. $bkey]));
    }
    $rows[] = $row;
  }

  // Theme as table.
  $table = theme('table', $header, $rows);
  $table .= drupal_render($form['exclude']['submit']);

  $fieldset = array(
    '#title' => t('Exclude matrix'),
    '#description' => t('Toggle per content type which build modes you want to exclude from rendering by Node Displays. <br />The "Toggle all" checkbox will exclude/include all build modes at once.'),
    '#children' => $table,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $output = theme('fieldset', $fieldset);

  $output .= drupal_render($form);
  return $output;
}

/**
 * Submit build mode matrix form.
 */
function ds_build_modes_matrix_submit($form, &$form_state) {
  $exclude_build_modes = array();
  foreach ($form['#content_types'] as $ckey => $type) {
    $global_exclude = FALSE;
    foreach ($form['#build_modes'] as $bkey => $mode) {
      // Global exclude.
      if ($bkey == 'nd_block_all') {
        $global_exclude = $form_state['values'][$ckey .'-'. $bkey];
        variable_set('nd_contenttype_'. $ckey, $form_state['values'][$ckey .'-'. $bkey]);
        $exclude_build_modes[$ckey][$bkey] = $form_state['values'][$ckey .'-'. $bkey];
      }
      else {
        $value = ($global_exclude == TRUE) ? $form_state['values'][$ckey .'-'. $bkey .'-disabled'] : $form_state['values'][$ckey .'-'. $bkey];
        $exclude_build_modes[$ckey][$bkey] = $value;
      }
    }
  }
  variable_set('nd_buildmodes_exclude', $exclude_build_modes);
  menu_rebuild();
}

/**
 * Build mode form.
 */
function ds_build_mode_form($form_state, $module, $build_mode = '') {
  $form = array();

  if (!empty($build_mode)) {
    $build_modes = variable_get($module .'_build_modes', array());
    $title = $build_modes[$build_mode];
  }
  else {
    $title = '';
  }

  $form['#module'] = $module;

  $form['identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($build_mode)) ? t('Add new build mode'): t('Update build mode'),
    '#collapsible' => (empty($build_mode)) ? TRUE : FALSE,
    '#collapsed' => (empty($build_mode)) ? TRUE : FALSE,
  );
  $form['identity']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'Key name',
    '#description' => t('The machine-readable name of this build mode.'),
    '#required' => TRUE,
  );
  if (!empty($build_mode)) {
    $form['identity']['key']['#disabled'] = TRUE;
    $form['identity']['key']['#value'] = $build_mode;
    $form['identity']['key']['#description'] = t('The machine-readable name of this build mode. Note: you can not edit this field.');
  }
  $form['identity']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Display name',
    '#description' => t('The name of this build mode which will be used for every content type on the display fields page.'),
    '#required' => TRUE,
    '#default_value' => $title,
  );
  $form['identity']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save display'),
    '#submit' => array('nd_build_mode_form_submit'),
  );
  $form['#form_type'] = (empty($build_mode)) ? 'insert' : 'update';

  return $form;
}

/**
 * Validate new custom display submission.
 */
function ds_build_mode_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $existing = array_keys(nd_get_build_modes($form['#module']));
    if (in_array($form_state['values']['key'], $existing)) {
      form_set_error('key', t('This display mode already exists.'));
    }

    if (!preg_match('!^[a-z_]+$!', $form_state['values']['key'])) {
      form_set_error('key', t('The machine-readable name must contain only lowercase letters and underscores.'));
    }
  }
}

/**
 * Save new build mode.
 */
function ds_build_mode_form_submit($form, &$form_state) {
  $build_modes = variable_get($form['#module'] .'_build_modes', array());
  $build_modes[$form_state['values']['key']] = $form_state['values']['name'];
  variable_set($form['#module'] .'_build_modes', $build_modes);
  drupal_set_message(t('Build mode %build_mode has been saved.', array('%build_mode' => $build_modes[$form_state['values']['key']])));
  $form_state['redirect'] = 'admin/content/types/nd/buildmodes';
  menu_rebuild();
}

/**
 * Build mode delete form.
 */
function ds_build_mode_delete_form($form_state, $module, $build_mode) {
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
}

/**
 * Delete display.
 */
function ds_build_mode_delete_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $build_modes = variable_get($module .'_build_modes', array());
  unset($build_modes[$form_state['values']['display']]);
  variable_set($module .'_build_modes', $build_modes);
  drupal_set_message(t('Custom build mode %build_mode has been removed.', array('%build_mode' => $form_state['values']['display'])));
  $form_state['redirect'] = 'admin/ds/'. $module .'/buildmodes';
  menu_rebuild();
}
