<?php

/**
 * Administrative functions for Display suite forms.
 */

/**
 * Return the forms.
 */
function _ds_forms_ds_forms_info() {
  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity) {

    // We only work on node forms now.
    if ($entity_type != 'node') {
      continue;
    }

    // Fieldable only.
    if ((isset($entity['fieldable']) && $entity['fieldable']) || isset($entity['ds_display'])) {
      foreach ($entity['bundles'] as $bundle_type => $bundle) {

        $path = isset($bundle['admin']['real path']) ? $bundle['admin']['real path'] : (isset($bundle['admin']['path']) ? $bundle['admin']['path'] : '');
        if (empty($path)) {
          continue;
        }

        $forms[$entity_type . '_' . $bundle_type] = array(
          'group' => $entity_type,
          'label' => $bundle['label'],
          'path' => $path . '/fields',
        );
      }
    }
  }

  return $forms;
}

/**
 * List callback to display all forms.
 */
function ds_forms_list() {
  $rows = array();
  $forms = ds_forms_get_forms(TRUE);

  // TODO check with different in other.
  foreach ($forms as $form_id => $form_values) {

    // Create a group if necessary.
    $group = $form_values['group'];
    if (!isset($rows['group'])) {
      $label = check_plain(drupal_ucfirst($group));
      $rows[$group] = array(array('data' => $label, 'colspan' => '2', 'style' => 'background-color: #E1E2DC; border: 1px solid #BEBFB9;'));
    }

    $rows[] = array(
      0 => $form_values['label'],
      1 => l(t('Manage form'), $form_values['path']),
    );
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