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
  $entity_info = entity_get_info();

  // Move node to the top.
  if (isset($entity_info['node'])) {
    $node_entity = $entity_info['node'];
    unset($entity_info['node']);
    $entity_info = array_merge(array('node' => $node_entity), $entity_info);
  }

  if (isset($entity_info['comment'])) {
    $comment_entity = $entity_info['comment'];
    unset($entity_info['comment']);
    $entity_info['comment'] = $comment_entity;
  }

  foreach ($entity_info as $entity_type => $info) {
    if (!empty($info['fieldable']) && !empty($info['base_table'])) {
      $rows = array();
      $bundles = entity_get_bundles($entity_type);
      foreach ($bundles as $bundle_type => $bundle) {
        $row = array();

        $path = isset($bundle['admin']['real path']) ? $bundle['admin']['real path'] : (isset($bundle['admin']['path']) ? $bundle['admin']['path'] : '');
        if (empty($path)) {
          continue;
        }

        $row[] = $bundle['label'];

        $operations['manage_display'] = array(
          'title' => t('Manage display'),
          'href' => $path . '/display',
        );
        // Add Mangage Form link if Display Suite Forms is enabled.
        if (module_exists('ds_forms')) {
          $operations['manage_form'] = array(
            'title' => t('Manage form'),
            'href' => $path . '/fields',
          );
        }
        // Add operation links.
        $row[] = array(
          'data' => array(
            '#type' => 'operations',
            '#subtype' => 'ds',
            '#links' => $operations,
          ),
        );

        $rows[] = $row;
      }

      if (!empty($rows)) {
        $variables = array(
          'header' => array(array('data' => $info['label']), array('data' => t('operations'), 'class' => 'ds-display-list-options')),
          'rows' => $rows,
        );
        $build['list_' . $entity_type] = array(
          '#markup' => theme('table', $variables)
        );

        $info_rows = array();
        $rows = array();
      }
    }
  }

  $build['#attached']['css'] = array(drupal_get_path('module', 'ds') . '/css/ds.admin.css');

  return $build;
}

/**
 * Emergency page.
 */
function ds_emergency() {

  $form['ds_fields_error'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields error'),
  );

  $form['ds_fields_error']['ds_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable attaching fields via Display Suite'),
    '#description' => t('In case you get an error after configuring a layout printing a message like "Fatal error: Unsupported operand types", you can temporarily disable adding fields from DS by toggling this checkbox. You probably are trying to render an node inside a node, for instance through a view, which is simply not possible. See <a href="http://drupal.org/node/1264386">http://drupal.org/node/1264386</a>.'),
    '#default_value' => config('ds.settings')->get('disable', FALSE),
    '#weight' => 0,
  );

  $form['ds_fields_error']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Disable/enable field attach'),
    '#submit' => array('ds_emergency_fields_attach'),
    '#weight' => 1,
  );

  if (module_exists('ds_extras')) {
    $region_blocks = config('ds.extras')->get('region_blocks', array());
    if (!empty($region_blocks)) {

      $region_blocks_options = array();
      foreach ($region_blocks as $key => $info) {
        $region_blocks_options[$key] = $info['title'];
      }

      $form['region_to_block'] = array(
        '#type' => 'fieldset',
        '#title' => t('Block regions'),
      );

      $form['region_to_block']['remove_block_region'] = array(
        '#type' => 'checkboxes',
        '#options' => $region_blocks_options,
        '#description' => t('In case you renamed a content type, you will not see the configured block regions anymore, however the block on the block settings page is still available. On this screen you can remove orphaned block regions.'),
      );

      $form['region_to_block']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update block regions'),
        '#submit' => array('ds_emergency_region_to_block_submit'),
        '#weight' => 1,
      );
    }
  }

  return $form;
}

/**
 * Submit callback of fields error form.
 */
function ds_emergency_fields_attach($form, &$form_state) {
  config('ds.settings')->set('disable', $form_state['values']['disable'])->save();
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Submit callback of region to block form.
 */
function ds_emergency_region_to_block_submit($form, &$form_state) {
  if (isset($form_state['values']['remove_block_region'])) {
    $save = FALSE;
    $region_blocks = config('ds.extras')->get('region_blocks', array());
    $remove = $form_state['values']['remove_block_region'];
    foreach ($remove as $key => $value) {
      if ($key === $value) {
        $save = TRUE;
        db_delete('block')
          ->condition('delta', $key)
          ->condition('module', 'ds_extras')
          ->execute();
        unset($region_blocks[$key]);
      }
    }

    if ($save) {
      drupal_set_message(t('Block regions were removed.'));
      config('ds.extras')->et('region_blocks', $region_blocks);
    }
  }
  else {
    drupal_set_message(t('No block regions were removed.'));
  }
}
