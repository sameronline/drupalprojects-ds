<?php

/**
 * @file
 * Shows the overview screen with all links to entities.
 */

/**
 * Menu callback: Show the layout list.
 */
function ds_layout_list() {
  $build = array();

  // All entities.
  $rows = array();
  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity) {
    if ((isset($entity['fieldable']) && $entity['fieldable']) || isset($entity['ds_display'])) {

      $entity_rows = array();
      foreach ($entity['bundles'] as $bundle_type => $bundle) {
        $row = array();

        $path = isset($bundle['admin']['real path']) ? $bundle['admin']['real path'] : (isset($bundle['admin']['path']) ? $bundle['admin']['path'] : '');
        if (empty($path)) {
          continue;
        }

        $row[] = $bundle['label'];
        if (module_exists('ds_forms')) {
          if ($entity_type == 'node') {
            $row[] = l(t('Manage forms'), $path . '/fields');
          }
          else {
            $row[] = '';
          }
        }
        $row[] = l(t('Manage display'), $path . '/display');
        $entity_rows[] = $row;
      }

      if (!empty($entity_rows)) {
        $rows[] = array(array('data' => $entity['label'], 'colspan' => '3', 'style' => 'background-color: #E1E2DC; border: 1px solid #BEBFB9;'));
        $rows = array_merge($rows, $entity_rows);
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
    $build['list'] = array('#markup' => t('No entities found'));
  }

  return $build;
}
