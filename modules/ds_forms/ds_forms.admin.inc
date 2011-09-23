<?php

/**
 * Administrative functions for Display suite forms.
 */

/**
 * Forms info hook.
 */
function _ds_forms_ds_forms_info() {
  $forms = array();

  // We currently only work on node types.
  foreach (node_type_get_types() as $type) {

    $elements = array();

    // Action buttons.
    $elements['actions'] = t('Actions');

    // Additional settings is exposed if field group is not enabled.
    if (!module_exists('field_group')) {
      $elements['additional_settings'] = t('Additional settings');
    }

    // Translation support.
    if (module_exists('translation') && translation_supported_type($type->type)) {
      $elements['language'] = t('Language');
    }

    $forms[$type->type . '_node_form'] = array(
      'entity_type' => 'node',
      'bundle' => $type->type,
      'label' => $type->name,
      'path' => 'admin/structure/types/manage/' . $type->type . '/fields',
      'elements' => $elements,
    );
  }

  return $forms;
}

/**
 * List callback to display all forms.
 */
function ds_forms_list() {
  $rows = $groups = array();
  $forms = ds_forms_get_forms(TRUE);

  foreach ($forms as $form_id => $form_values) {

    // Create a group.
    $group = $form_values['entity_type'];
    if (!isset($groups[$group])) {
      if ($group == 'ds_forms') {
        $label = t('Other');
      }
      else {
        $label = check_plain(drupal_ucfirst(str_replace('_', ' ', $group)));
      }
      $groups[$group]['header'] = array(array('data' => $label, 'colspan' => '2', 'style' => 'background-color: #E1E2DC; border: 1px solid #BEBFB9;'));
    }

    $path = isset($form_values['path']) ? $form_values['path'] : 'admin/structure/ds/forms/manage/' . $form_id;
    $groups[$group]['forms'][$form_id] = array(
      0 => $form_values['label'],
      1 => l(t('Manage form'), $path),
    );
  }

  if (!empty($groups)) {
    sort($groups);
    foreach ($groups as $group => $data) {
      $rows[] = $data['header'];
      foreach ($data['forms'] as $form_id => $value) {
        $rows[] = $value;
      }
    }
  }

  if (!empty($rows)) {
    $variables = array(
      'header' => array(),
      'rows' => $rows,
    );
    $build['list'] = array('#markup' => theme('table', $variables));
  }
  else {
    // This will probably never happen.
    $build['list'] = array('#markup' => t('No forms found'));
  }

  return $build;
}

/**
 * Page callback to manage a custom form through Display suite.
 *
 * @param $form
 *   The configuration of the form.
 */
function ds_forms_manage_custom($form) {
  $form_id = $form['form_id'];

  // Use drupal_build_form instead of drupal_get_form.
  $form_state = array();
  $arguments = array('ds_forms', $form_id, 'form');
  $form_state['build_info']['args'] = $arguments;
  $form_state['no_panels'] = TRUE;
  $form_state['no_view_mode_suggestions'] = TRUE;
  form_load_include($form_state, 'inc', 'field_ui', 'field_ui.admin');
  $build = drupal_build_form('field_ui_field_overview_form', $form_state);

  // Deny access to fields.
  unset($build['fields']['_add_new_field']);
  unset($build['fields']['_add_existing_field']);

  // @todo - test this if this could actually work (it should).
  if (module_exists('field_group')) {
    unset($build['fields']['_add_new_group']);
  }

  // Deny access to fields table if there's no layout.
  if (!ds_get_layout('ds_forms', $form_id, 'form')) {
    $build['fields']['#access'] = FALSE;
  }

  return $build;
}
