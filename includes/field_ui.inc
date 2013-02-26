<?php

/**
 * @file
 * Field UI functions for Display Suite.
 */

use Drupal\field_ui\DisplayOverview;

/**
 * Adds the Display Suite fields and layouts to the form.
 */
function ds_field_ui_fields_layouts(&$form, &$form_state) {
  global $base_root, $base_path;

  // Get the entity_type, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  $form['#export_id'] = $entity_type . '|' . $bundle . '|' . $view_mode;

  // Create vertical tabs.
  ds_field_ui_create_vertical_tabs($form);

  // Add layout fieldset.
  _ds_field_ui_table_layouts($entity_type, $bundle, $view_mode, $form, $form_state);

  // Add/alter fields on the table, but only if a layout is selected.
  if ($view_mode != 'form' && !empty($form['#ds_layout'])) {
    _ds_field_ui_fields($entity_type, $bundle, $view_mode, $form, $form_state);

    // Also alter core fields
    _ds_field_ui_core_fields($entity_type, $bundle, $view_mode, $form, $form_state);
  }

  // Special validate function for field group.
  if (isset($form_state['no_field_group'])) {
    array_unshift($form['#validate'], '_ds_field_group_field_ui_fix_notices');
  }

  // Attach js.
  $form['#attached']['js'][] = drupal_get_path('module', 'ds') . '/js/ds.admin.js';

  // Attach css.
  $form['#attached']['css'][] = drupal_get_path('module', 'ds') . '/css/ds.admin.css';

  // Add process function to add the regions.
  $form['#process'][] = 'ds_field_ui_regions';

  // Add a destination so we can get back if layout has been changed.
  $form['ds_source'] = array(
    '#type' => 'hidden',
    '#value' => $base_root . $base_path,
  );
  $form['ds_destination'] = array(
    '#type' => 'hidden',
    '#value' => drupal_get_destination(),
  );
  $form['ds_entity_type'] = array(
    '#type' => 'hidden',
    '#value' => $entity_type,
  );
  $form['ds_bundle'] = array(
    '#type' => 'hidden',
    '#value' => $bundle,
  );
  $form['ds_view_mode'] = array(
    '#type' => 'hidden',
    '#value' => $view_mode,
  );
}

/**
 * Create vertical tabs.
 */
function ds_field_ui_create_vertical_tabs(&$form) {

  // Add additional settings vertical tab.
  if (!isset($form['additional_settings'])) {
    $form['additional_settings'] = array(
      '#type' => 'vertical_tabs',
      '#theme_wrappers' => array('vertical_tabs'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
  }

  $view_mode_admin_access = user_access('admin_view_modes') && module_exists('ds_ui');
  if (isset($form['modes'])) {
    $form['modes']['#group'] = 'additional_settings';
    $form['modes']['#weight'] = -10;
    if ($view_mode_admin_access) {
      $form['modes']['view_modes_custom']['#description'] = l(t('Manage view modes'), 'admin/structure/ds/view_modes');
    }
  }
  else {
    if ($view_mode_admin_access) {
      $form['modes'] = array(
        '#type' => 'details',
        '#title' => t('Custom display settings'),
        '#group' => 'additional_settings',
      );
      $form['modes']['view_modes_custom'] = array(
       '#markup' => l(t('Manage view modes'), 'admin/structure/ds/view_modes'),
      );
    }
  }
}

/**
 * Add Regions to 'Manage fields' or 'Manage display' screen.
 *
 * @param $form
 *   The form to add layout fieldset and extra Display Suite fields.
 * @param $form_state
 *   The current form state.
 */
function ds_field_ui_regions($form, $form_state) {

  // Get the entity_type, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Ignore fieldgroup options.
  if (isset($form_state['no_field_group'])) {
    unset($form['fields']['_add_new_group']);
    $form['field_group']['#access'] = FALSE;
  }

  // Check layout.
  $layout = isset($form['#ds_layout']) ? $form['#ds_layout'] : FALSE;

  // Change UI to add Region column if we have a layout.
  if ($layout) {
    $table = &$form['fields'];

    if ($view_mode != 'form') {
      $table['#header'] = array(
        t('Field'),
        t('Weight'),
        t('Parent'),
        t('Region'),
        t('Label'),
        array('data' => t('Format'), 'colspan' => 3),
      );
    }
    else {
      $table['#header'] = array(
        t('Label'),
        t('Weight'),
        t('Parent'),
        t('Region'),
        t('Name'),
        t('Field'),
        t('Widget'),
        array('data' => t('Operations'), 'colspan' => 2),
      );
    }

    // Remove label and format for views.
    if ($entity_type == 'ds_views') {
      $table['#header'][4] = '';
    }

    $table['#regions'] = array();
    foreach ($layout['regions'] as $region_key => $region_title) {
      $region_options[$region_key] = $region_title;
      $table['#regions'][$region_key] = array(
        'title' => $region_title,
        'message' => t('No fields are displayed in this region'),
      );
    }

    // Let other modules alter the regions.
    $context = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'view_mode' => $view_mode
    );
    $region_info = array(
      'region_options' => &$region_options,
      'table_regions' => &$table['#regions'],
    );
    drupal_alter('ds_layout_region', $context, $region_info);

    $region_options['hidden'] = $view_mode != 'form' ? t('Disabled') : t('Hidden');
    $table['#regions']['hidden'] = array(
      'title' => $view_mode != 'form' ? t('Disabled') : t('Hidden'),
      'message' => t('No fields are hidden.')
    );

    $region = array(
      '#type' => 'select',
      '#options' => $region_options,
      '#default_value' => 'hidden',
      '#attributes' => array(
        'class' => array('ds-field-region'),
      )
    );

    $limit_items = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#default_value' => '',
      '#weight' => 10,
      '#default_value' => '#',
      '#prefix' => '<div class="limit-float">',
      '#suffix' => '</div><div class="clearfix"></div>',
      '#attributes' => array(
        'alt' => t('Enter a number to limit the number of items or \'delta\' to print a specific delta (usually configured in views or found in entity->ds_delta). Leave empty to display them all. Note that depending on the formatter settings, this option might not always work.'),
        'title' => t('Enter a number to limit the number of items or \'delta\' to print a specific delta (usually configured in views or found in entity->ds_delta). Leave empty to display them all. Note that depending on the formatter settings, this option might not always work.'),
      ),
    );

    // Hide this if we formatter_settings_edit is not empty so it doesn't confuse users.
    if (!empty($form_state['formatter_settings_edit'])) {
      $limit_items['#access'] = FALSE;
    }

    // Update existing rows by changing rowHandler and adding regions.
    foreach (element_children($table) as $name) {
      $row = &$table[$name];
      $row['#js_settings'] = array('rowHandler' => 'ds');
      $row['#region_callback'] = 'ds_field_ui_row_region';

      // Remove hidden format.
      if (isset($row['format']['type']['#options']['hidden'])) {
        unset($row['format']['type']['#options']['hidden']);
      }

      // Add label class.
      if (isset($row['label'])) {
        if (isset($form_state['formatter_settings']) && isset($form_state['formatter_settings'][$name]['ft'])) {
          if (!empty($form_state['formatter_settings'][$name]['ft']['lb'])) {
            $row['human_name']['#markup'] = check_plain($form_state['formatter_settings'][$name]['ft']['lb']) . ' ' . t('(Original: !orig)', array('!orig' => $row['human_name']['#markup']));
          }
        }
      }

      // Limit items.
      $field_info = field_info_field($name);
      if (isset($field_info['cardinality']) && $field_info['cardinality'] != 1 && $view_mode != 'form') {
        $row['format']['type']['#prefix'] = '<div class="limit-float">';
        $row['format']['type']['#suffix'] = '</div>';
        $row['format']['limit'] = $limit_items;
        $row['format']['limit']['#default_value'] = (isset($layout['settings']['limit']) && isset($layout['settings']['limit'][$name])) ? $layout['settings']['limit'][$name] : '#';
      }

      // Disable label and format for views.
      if ($entity_type == 'ds_views') {
        $row['label']['#access'] = FALSE;
      }

      // Add region.
      $split = ($view_mode != 'form') ? 7 : 6;
      if ($row['#row_type'] == 'group' && $view_mode == 'form') {
        $split = ($view_mode != 'form') ? 8 : 7;
      }
      $second = array_splice($row, $split);
      $row['region'] = $region;
      $row['region']['#default_value'] = (isset($layout['settings']['fields'][$name]) && isset($region_options[$layout['settings']['fields'][$name]])) ? $layout['settings']['fields'][$name] : 'hidden';
      $row = array_merge($row, $second);
    }
  }

  return $form;
}

/**
 * Returns the region to which a row in the Field UI screen belongs.
 *
 * @param $row
 *   The current row that is being rendered in the Field UI screen.
 */
function ds_field_ui_row_region($row) {
  return isset($row['region']['#value']) ? $row['region']['#value'] : 'hidden';
}

/**
 * Move the view modes so Field UI can handle them.
 */
function ds_field_ui_layouts_validate($form, &$form_state) {
  if (isset($form_state['values']['modes']['view_modes_custom'])) {
    $form_state['values']['view_modes_custom'] = $form_state['values']['modes']['view_modes_custom'];
  }
}

/**
 * Change a layout for a given entity.
 *
 * @param $entity_type
 *   The name of the entity.
 * @param $bundle
 *   The name of the bundle.
 * @param $view_mode
 *   The name of the view mode.
 */
function ds_field_ui_layout_change($form, $form_state, $entity_type = '', $bundle = '', $view_mode = '', $new_layout = '') {

  $old_layout = NULL;
  $all_layouts = ds_get_layout_info();

  if (!empty($entity_type) && !empty($bundle) && !empty($view_mode)) {
    $old_layout = ds_get_layout($entity_type, $bundle, $view_mode, FALSE);
  }

  if ($old_layout && isset($all_layouts[$new_layout])) {

    $new_layout_key = $new_layout;
    $new_layout = $all_layouts[$new_layout];

    $form['#entity_type'] = $entity_type;
    $form['#bundle'] = $bundle;
    $form['#view_mode'] = $view_mode;
    $form['#old_layout'] = $old_layout;
    $form['#new_layout'] = $new_layout;
    $form['#new_layout_key'] = $new_layout_key;
    $form['#export_id'] = $entity_type . '|' . $bundle . '|' . $view_mode;

    $form['info'] = array(
      '#markup' => t('You are changing from %old to %new layout for !bundle in !view_mode view mode.', array('%old' => $old_layout['label'], '%new' => $new_layout['label'], '!bundle' => $bundle, '!view_mode' => $view_mode)),
      '#prefix' => "<div class='change_ds_layout_info'>",
      '#suffix' => "</div>",
    );

    // Old region options.
    $regions = array();
    foreach ($old_layout['regions'] as $key => $title) {
      $regions[$key] = $title;
    }

    // Let other modules alter the regions.
    // For old regions.
    $context = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'view_mode' => $view_mode,
    );
    $region_info = array(
      'region_options' => $regions,
    );
    drupal_alter('ds_layout_region', $context, $region_info);
    $regions = $region_info['region_options'];
    $form['#old_layout']['regions'] = $regions;

    // For new regions.
    $region_info = array(
      'region_options' => $new_layout['regions'],
    );
    drupal_alter('ds_layout_region', $context, $region_info);
    $new_layout['regions'] = $region_info['region_options'];
    $form['#new_layout']['regions'] = $new_layout['regions'];

    // Display the region options
    $selectable_regions = array('' => t('- None -')) + $new_layout['regions'];
    $form['regions_pre']['#markup'] = '<div class="ds-layout-regions">';
    foreach ($regions as $region => $region_title) {
      $form['region_' . $region] = array(
        '#type' => 'container',
      );
      $form['region_' . $region]['ds_label_' . $region] = array(
        '#markup' => 'Fields in <span class="change_ds_layout_old_region"> ' . $region_title . '</span> go into',
      );
      $form['region_' . $region]['ds_' . $region] = array(
        '#type' => 'select',
        '#options' => $layout_options = $selectable_regions,
        '#default_value' => $region,
      );
    }
    $form['regions_post']['#markup'] = '</div>';

    // Show previews from old and new layouts
    $form['preview'] = array(
      '#type' => 'container',
      '#prefix' => '<div class="ds-layout-preview"/>',
      '#suffix' => '</div>',
    );

    $fallback_image = drupal_get_path('module', 'ds') . '/images/preview.png';
    $old_image = (isset($old_layout['image']) &&  !empty($old_layout['image'])) ? $old_layout['path'] . '/' . $old_layout['layout'] . '.png' : $fallback_image;
    $new_image = (isset($new_layout['image']) &&  !empty($new_layout['image'])) ? $new_layout['path'] . '/' . $new_layout_key . '.png' : $fallback_image;
    $arrow = drupal_get_path('module', 'ds') . '/images/arrow.png';

    $form['preview']['old_layout'] = array(
      '#markup' => '<div class="ds-layout-preview-image"><img src="' . base_path() . $old_image . '"/></div>',
    );
    $form['preview']['arrow'] = array(
      '#markup' => '<div class="ds-layout-preview-arrow"><img src="' . base_path() . $arrow . '"/></div>',
    );
    $form['preview']['new_layout'] = array(
      '#markup' => '<div class="ds-layout-preview-image"><img src="' . base_path() . $new_image . '"/></div>',
    );
    $form['#attached']['css'][] = drupal_get_path('module', 'ds') . '/css/ds.admin.css';

    // Submit button
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#prefix' => '<div class="ds-layout-change-save">',
      '#suffix' => '</div>',
    );
  }
  else {
    $form['nothing'] = array('#markup' => t('No valid configuration found.'));
  }

  return $form;
}

/**
 * Submit callback: save the layout change.
 */
function ds_field_ui_layout_change_submit($form, &$form_state) {

  // Prepare some variables.
  $old_layout = $form['#old_layout'];
  $new_layout = $form['#new_layout'];
  $new_layout_key = $form['#new_layout_key'];
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Create new record.
  $record = array();
  $record['id'] = $form['#export_id'];
  $record['entity_type'] = $entity_type;
  $record['bundle'] = $bundle;
  $record['view_mode'] = $view_mode;
  $record['layout'] = $new_layout_key;
  $record['settings'] = $old_layout['settings'];
  unset($record['settings']['regions']);
  unset($record['settings']['fields']);

  // map old regions to new ones
  foreach ($old_layout['regions'] as $region => $region_title) {
    $new_region = $form_state['values']['ds_' . $region];
    if ($new_region != '' && isset($old_layout['settings']['regions'][$region])) {
      foreach ($old_layout['settings']['regions'][$region] as $field_key => $field) {
        if (!isset($record['settings']['regions'][$new_region])) {
          $record['settings']['regions'][$new_region] = array();
        }
        $record['settings']['regions'][$new_region][] = $field;
        $record['settings']['fields'][$field] = $new_region;
      }
    }
  }

  // Save configuration.
  config('ds.layout_settings.' . $entity_type . '.' . $bundle . '.' . $view_mode)->setData($record)->save();

  // Clear entity info cache.
  entity_info_cache_clear();

  // Show message.
  drupal_set_message(t('The layout change has been saved.'));
}

/**
 * Save the layout settings from the 'Manage display' screen.
 */
function ds_field_ui_layouts_save($form, &$form_state) {
  $weight = 0;

  // Get default values.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Determine layout variables.
  $layout = $form_state['values']['layout'];
  $old_layout = $form_state['values']['old_layout'];
  $new_layout = ($layout != $old_layout) || empty($old_layout);

  // Save layout and add regions if necessary.
  $record = array();
  $record['id'] = $form['#export_id'];
  $record['entity_type'] = $entity_type;
  $record['bundle'] = $bundle;
  $record['view_mode'] = $view_mode;
  $record['layout'] = $layout;
  $record['settings'] = array();

  // Remove old layout if necessary.
  if ($new_layout && !empty($old_layout)) {
    config('ds.layout_settings.' . $entity_type . '.' . $bundle . '.' . $view_mode)->delete();
  }

  if ($new_layout && !empty($layout)) {
    // Save new layout.
    $record['settings'] = $record['settings'];

    // Let other modules alter the layout settings.
    drupal_alter('ds_layout_settings', $record, $form_state);

    // Move current visible fields into a default region, so
    // we keep their current settings.
    $layouts = ds_get_layout_info();
    $sl = $layouts[$layout];
    $first_region = key($sl['regions']);
    $record['settings']['regions'] = array();
    $record['settings']['fields'] = array();
    $record['settings']['classes'] = array();
    $record['settings']['wrappers'] = array();
    $record['settings']['layout_wrapper'] = 'div';
    $record['settings']['layout_attributes'] = '';
    $record['settings']['layout_attributes_merge'] = variable_get('ds_layout_attributes_merge', TRUE);;
    $fields = _ds_sort_fields($form_state['values']['fields'], 'weight');
    foreach ($fields as $field_key => $field) {

      // Ignore new fieldgroup, new field or existing field.
      if (in_array($field_key, array('_add_new_field', '_add_existing_field', '_add_new_group'))) {
        continue;
      }

      // Can either be form or display.
      if ((isset($field['type']) && $field['type'] != 'hidden') || $record['view_mode'] == 'form') {
        $record['settings']['regions'][$first_region][$weight++] = $field_key;
        $record['settings']['fields'][$field_key] = $first_region;
      }
    }
    // In case this is the full node view mode and if the comment module
    // is enabled for this content type, add it as well.
    if ($record['entity_type'] == 'node' && $record['view_mode'] == 'full' && module_exists('comment')) {
      $record['settings']['regions'][$first_region][] = 'comments';
      $record['settings']['fields']['comments'] = $first_region;
    }

    // Save the configuration.
    config('ds.layout_settings.' . $entity_type . '.' . $bundle . '.' . $view_mode)->setData($record)->save();
  }
  // Update existing layout.
  elseif (!empty($layout)) {

    $fields = _ds_sort_fields($form_state['values']['fields'], 'weight');

    foreach ($fields as $key => $field) {

      // Make sure we need to save anything for this field.
      if (_ds_field_valid($key, $field, $form_state, $view_mode)) {
        continue;
      }

      if (!isset($record['settings']['regions'][$field['region']])) {
        $record['settings']['regions'][$field['region']] = array();
      }
      $record['settings']['regions'][$field['region']][$weight++] = $key;
      $record['settings']['fields'][$key] = $field['region'];

      // Save limit.
      $limit = isset($field['format']['limit']) ? trim($field['format']['limit']) : '';
      if (is_numeric($limit) || $limit === 'delta') {
        $record['settings']['limit'][$key] = $limit;
      }
    }

    // Save the region classes.
    $record['settings']['classes'] = array();
    foreach (array_keys($form['fields']['#regions']) as $region) {

      // Ignore hidden region.
      if ($region == 'hidden') {
        continue;
      }

      if (isset($form_state['values']['layout_class'])) {
        $record['settings']['classes']['layout_class'] = $form_state['values']['layout_class'];
      }

      // Additional classes on regions.
      if (isset($form_state['values'][$region])) {
        // Do not save empty string.
        $classes = is_array($form_state['values'][$region]) ? implode(' ', $form_state['values'][$region]) : array();
        if (!empty($classes)) {
          $record['settings']['classes'][$region] = $form_state['values'][$region];
        }
      }

      // Additional wrappers on regions.
      if (isset($form_state['values']['region_wrapper'][$region])) {
        $record['settings']['wrappers'][$region] = $form_state['values']['region_wrapper'][$region];
      }
    }

    // Layout wrapper
    $record['settings']['layout_wrapper'] = $form_state['values']['layout_wrapper'];
    $record['settings']['layout_attributes'] = filter_xss_admin($form_state['values']['layout_attributes']);
    $record['settings']['layout_attributes_merge'] = $form_state['values']['layout_attributes_merge'];

    $record['settings'] = $record['settings'];

    // Let other modules alter the layout settings.
    drupal_alter('ds_layout_settings', $record, $form_state);

    // Save the configuration.
    config('ds.layout_settings.' . $entity_type . '.' . $bundle . '.' . $view_mode)->setData($record)->save();

    // Clear entity info cache.
    entity_info_cache_clear();
  }
}

/**
 * Form validation handler for _ds_field_ui_fields().
 */
function ds_field_ui_fields_validate($form, &$form_state) {
  foreach (element_children($form['fields']) as $key) {
    if (isset($form_state['values']['fields'][$key]['settings_edit_form'])) {
      $settings = $form_state['values']['fields'][$key]['settings_edit_form']['settings']['ft'];
      if (!empty($settings)) {
        $merge = isset($form_state['formatter_settings'][$key]['ft']) ? $form_state['formatter_settings'][$key]['ft'] : array();
        $form_state['formatter_settings'][$key]['ft'] = array_merge($merge, $settings);
      }
    }
  }
}

/**
 * Save the field settings from the 'Manage display' screen.
 */
function ds_field_ui_fields_save($form, &$form_state) {
  // Setup some variables.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  $field_settings = array();

  // Save settings for each field.
  $fields = $form['#ds_fields'];
  foreach ($fields as $key => $field) {

    // Field settings.
    $field_values = $form_state['values']['fields'][$field];

    // In case the region is hidden, do not save.
    if (isset($field_values['region']) && $field_values['region'] == 'hidden') {
      continue;
    }

    // Build settings.
    $settings = array();
    $settings['weight'] = $field_values['weight'];
    $settings['label'] = $field_values['label'];
    $settings['format'] = $field_values['format']['type'];

    // Any formatter settings.
    if (isset($form_state['formatter_settings'][$field])) {
      $settings['formatter_settings'] = $form_state['formatter_settings'][$field];
    }

    $field_settings[$field] = $settings;
  }

  // Allow other modules to modify the field settings before they get saved.
  drupal_alter('ds_field_settings', $field_settings, $form, $form_state);

  // Save the record.
  if (!empty($field_settings)) {
    $config = array();
    $config['id'] = $form['#export_id'];
    $config['entity_type'] = $entity_type;
    $config['bundle'] = $bundle;
    $config['view_mode'] = $view_mode;
    $config['settings'] = $field_settings;
    config('ds.field_settings.' . $entity_type . '.' . $bundle . '.' . $view_mode)->setData($config)->save();
  }

  // Clear the ds_fields cache.
  cache()->invalidateTags(array('ds_fields' => TRUE));
}

/**
 * Clone a fields layout.
 */
function ds_field_ui_layout_clone($form, &$form_state) {

  $clone = $form_state['values']['clone'];
  list($ce, $cb, $cv) = explode('|', $clone);
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  $layout = config('ds.layout_settings'. $ce  . '.' . $cb . '.' . $cv)->get();

  // Delete previous field configuration - @todo where's the clone anyway ?
  config('ds.field_settings.' . $entity_type . '.' . $bundle . '.' . $view_mode)->delete();

  // Save new layout record for ds.
  if ($layout) {
    $record = new stdClass();
    $record['id'] = $form['#export_id'];
    $record['entity_type'] = $entity_type;
    $record['bundle'] = $bundle;
    $record['view_mode'] = $view_mode;
    $record['layout'] = $layout['layout'];
    $record['settings'] = $layout['settings'];

    // Let other modules alter the layout settings.
    drupal_alter('ds_layout_settings', $record, $form_state);

    // Save layout record.
    config('ds.layout_settings' . '.' . $entity_type . '.' . $bundle . '.' . $view_mode)->setData($record)->save();

    // Copy the view mode settings.
    _ds_field_ui_clone_view_mode_settings($entity_type, $bundle, $view_mode, $cv);

    // Clear entity info cache.
    entity_info_cache_clear();

    // Show message.
    drupal_set_message(t('The layout has been cloned.'));

  }
  else {
    drupal_set_message(t('No layout was cloned.'));
  }
}

/**
 * Populates display settings for a new view mode from the another view mode.
 *
 * This is almost a straight copy from Field UI, but with the addition
 * that we can pass the view mode from which we want to clone from.
 */
function _ds_field_ui_clone_view_mode_settings($entity_type, $bundle, $view_mode, $copy_view_mode) {

  $settings = field_bundle_settings($entity_type, $bundle);

  // Update display settings for field instances.
  $instances = field_read_instances(array('entity_type' => $entity_type, 'bundle' => $bundle));
  foreach ($instances as $instance) {
    // If this field instance has display settings defined for this view mode,
    // respect those settings.
    $instance['display'][$view_mode] = $instance['display'][$copy_view_mode];
    field_update_instance($instance);
  }

  // Update display settings for 'extra fields'.
  foreach (array_keys($settings['extra_fields']['display']) as $name) {
    if (isset($settings['extra_fields']['display'][$name][$copy_view_mode])) {
      $settings['extra_fields']['display'][$name][$view_mode] = $settings['extra_fields']['display'][$name][$copy_view_mode];
    }
  }

  // Save the settings.
  field_bundle_settings($entity_type, $bundle, $settings);
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function ds_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  // Taxonomy view modes.
  if ($display['type'] === 'ds_taxonomy_view_mode') {
    $options = array();
    $view_modes = entity_get_view_modes('taxonomy_term');
    foreach ($view_modes as $key => $info) {
      $options[$key] = $info['label'];
    }
    $element['taxonomy_term_reference_view_mode'] = array(
      '#title'   => t('View mode'),
      '#type'    => 'select',
      '#options' => $options,
      '#default_value' => $settings['taxonomy_term_reference_view_mode'],
    );
    return $element;
  }

  // Taxonomy separated.
  if ($display['type'] === 'ds_taxonomy_separator') {
    $element['taxonomy_term_link'] = array(
      '#title' => t('Link to term'),
      '#type' => 'checkbox',
      '#size' => 10,
      '#default_value' => $settings['taxonomy_term_link'],
    );
    $separators = array(
      ' ' => t('space'),
      ', ' => t('comma'),
      ' - ' => t('dash'),
      ' / ' => t('slash'),
    );
    drupal_alter('ds_taxonomy_term_separators', $separators);
    $element['taxonomy_term_separator'] = array(
      '#title' => t('Separator'),
      '#type' => 'select',
      '#options' => $separators,
      '#default_value' => $settings['taxonomy_term_separator'],
      '#states' => array(
        'visible' => array(
          'select[name="fields[field_tags][settings_edit_form][settings][taxonomy_term_list]"]' => array('value' => 'separated_list'),
        ),
      ),
    );
    return $element;
  }
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function ds_field_formatter_settings_summary($field, $instance, $view_mode) {
  $summary = '';
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] === 'ds_taxonomy_view_mode') {
    $entity_info = entity_get_info('taxonomy_term');
    $modes = $entity_info['view modes'];
    $mode = $modes[$settings['taxonomy_term_reference_view_mode']]['label'];
    $summary = t('View mode: %mode', array('%mode' => $mode));
  }

  if ($display['type'] === 'ds_taxonomy_separator') {
    $separators = array(
      ' ' => t('space'),
      ', ' => t('comma'),
      ' - ' => t('dash'),
      ' / ' => t('slash'),
    );
    drupal_alter('ds_taxonomy_term_separators', $separators);
    $summary .= t('Separated by !sep', array('!sep' => $separators[$settings['taxonomy_term_separator']]));
    $summary .= $settings['taxonomy_term_link'] ? ', ' . t('linked') : ', ' . t('not linked');
  }

  return $summary;
}

/**
 * Creates a summary for the field format configuration summary.
 *
 * @param $field
 *   The configuration of the field.
 *
 * @return $summary
 *   An markup array.
 */
function ds_field_settings_summary($field, $form_state) {

  $summary = '';

  // Not all fields have settings.
  if (isset($field['properties']['settings'])) {
    $summary = module_invoke($field['module'], 'ds_field_format_summary', $field);
  }

  if (module_exists('ds_extras') && config('ds.extras')->get('field_template')) {
    module_load_include('inc', 'ds_extras', 'includes/admin');

    // Field template summary
    if (!in_array($field['field_type'], array(DS_FIELD_TYPE_IGNORE, DS_FIELD_TYPE_PREPROCESS))) {
      $functions = module_invoke_all('ds_field_theme_functions_info');
      $default_field_function = config('ds.extras')->get('ft-default');
      $field_function = isset($form_state['formatter_settings'][$field['name']]['ft']['func']) ? $form_state['formatter_settings'][$field['name']]['ft']['func'] : $default_field_function;

      $summary .= 'Field template: ' . check_plain($functions[$field_function]) . '<br />';
    }
  }

  if (empty($summary)) {
    return NULL; // no summary return nothing
  }

  return array(
    '#markup' => '<div class="field-formatter-summary">' . $summary . '</div>',
    '#cell_attributes' => array('class' => array('field-formatter-summary-cell')),
  );
}

/**
 * Creates a form for Display Suite fields.
 * .
 * @param $field
 *   The field definition.
 *
 * @return $form
 *   A form definition.
 */
function ds_field_settings_form($field, &$form_state) {
  $form = module_invoke($field['module'], 'ds_field_settings_form', $field);

  // Add field template settings to every field if enabled.
  if (module_exists('ds_extras') && config('ds.extras')->get('field_template', FALSE)) {
    $context = array(
      'instance' => array(
        'entity_type' => $field['entity_type'],
        'bundle' => $field['bundle'],
        'field_name' => $field['name'],
      ),
      'view_mode' => $field['view_mode'],
    );

    // Load the formatter settings form
    module_load_include('inc', 'ds_extras', 'includes/admin');
    // Protect against empty $form.
    if (!is_array($form)) $form = array();
    if (!in_array($field['field_type'], array(DS_FIELD_TYPE_IGNORE, DS_FIELD_TYPE_PREPROCESS))) {
      ds_extras_field_template_settings_form($form, $form_state, $context);
    }
    else {
      $form['#markup'] = t('This field does not support field templates.');
    }
  }

  return $form;
}

/**
 * Implements hook_ds_field_format_summary().
 */
function ds_ds_field_format_summary($field) {
  $summary = '';
  $settings = isset($field['formatter_settings']) ? $field['formatter_settings'] : $field['properties']['default'];
  $functions = module_invoke_all('ds_field_theme_functions_info');
  foreach ($settings as $key => $value) {
    if ($key == 'ft') {
      // Do nothing
    }
    elseif (!empty($value)) {
      $value = is_numeric($value) ? ($value ? t('Yes') : t('No')) : check_plain($value);
      $summary .= ' ' . str_replace('_', ' ', drupal_ucfirst(check_plain($key))) . ': ' . check_plain($value) . '<br />';
    }
  }

  return $summary;
}

/**
 * Implements hook_ds_field_settings_form().
 */
function ds_ds_field_settings_form($field) {
  $form = array();

  $settings = isset($field['formatter_settings']) ? $field['formatter_settings'] : $field['properties']['default'];

  foreach ($field['properties']['settings'] as $key => $value) {

    switch ($value['type']) {

      case 'textfield':
        $form[$key] = array(
          '#type' => 'textfield',
          '#title' => str_replace('_', ' ', check_plain(drupal_ucfirst($key))),
          '#default_value' => isset($settings[$key]) ? $settings[$key] : '',
          '#size' => 40,
          '#description' => isset($value['description']) ? check_plain($value['description']) : '',
        );
        break;

      case 'select':
        $form[$key] = array(
          '#type' => 'select',
          '#title' => check_plain(drupal_ucfirst($key)),
          '#default_value' => isset($settings[$key]) ? $settings[$key] : '',
          '#options' => $value['options'],
          '#description' => isset($value['description']) ? check_plain($value['description']) : '',
        );
        break;

      case 'checkbox':
        $form[$key] = array(
          '#type' => 'checkbox',
          '#title' => str_replace('_', ' ', check_plain(drupal_ucfirst($key))),
          '#default_value' => isset($settings[$key]) ? $settings[$key] : '',
          '#description' => isset($value['description']) ? check_plain($value['description']) : '',
        );
        break;
    }
  }

  return $form;
}

/**
 * Add fake field group value in.
 */
function _ds_field_group_field_ui_fix_notices($form, &$form_state) {
  $field_group = array(
    'group_name' => '',
    'label' => '',
  );
  $form_state['values']['fields']['_add_new_group'] = $field_group;
}

/**
 * Add the layouts fieldset on the Field UI screen.
 *
 * @param $entity_type
 *   The name of the entity type.
 * @param $bundle
 *   The name of the bundle
 * @param $view_mode
 *   The name of the view_mode
 * @param $form
 *   A collection of form properties.
 */
function _ds_field_ui_table_layouts($entity_type, $bundle, $view_mode, &$form, $form_state) {

  $layout_options = array();
  $ds_layouts = ds_get_layout_info();
  $layout_options = array('' => t('- None -'));
  foreach ($ds_layouts as $key => $layout) {
    $optgroup = 'Display Suite';

    // Create new layout option group.
    if (!empty($layout['module'])) {
      $optgroup = drupal_ucfirst($layout['module']);
    }

    if (!isset($layout_options[$optgroup])) {
      $layout_options[$optgroup] = array();
    }

    // Stack the layout.
    $layout_options[$optgroup][$key] = $layout['label'];
  }

  // If there is only one $optgroup, move it to the root.
  if (count($layout_options) == 2) {
    $options = $layout_options[$optgroup];
    $layout_options = array_merge(array('' => t('- None -')), $options);
  }

  // Add layouts form.
  $form['ds_layouts'] = array(
    '#type' => 'details',
    '#title' => t('Layout for !bundle in !view_mode', array('!bundle' => str_replace('_', ' ', $bundle), '!view_mode' => str_replace('_', ' ', $view_mode))),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
    '#collapsed' => FALSE,
    '#weight' => -100,
  );

  $layout = array();
  list($ce, $cb, $cv) = explode('|', $form['#export_id']);
  $layout = config('ds.layout_settings.' . $ce . '.' . $cb . '.' . $cv)->get();

  if (!empty($layout) && isset($layout['layout']) && isset($ds_layouts[$layout['layout']])) {
    $layout['settings'] = $layout['settings']; // @todo this does not make any sense.
    $layout['regions'] = $ds_layouts[$layout['layout']]['regions'];
    $form['#ds_layout'] = $layout;
  }

  // Load the layout preview form
  $layout['layout_options'] = $layout_options;
  _ds_field_ui_table_layouts_preview($form, $form_state, $ds_layouts, $layout, $entity_type, $bundle, $view_mode);

  if (!empty($layout) && isset($layout['regions'])) {

    // Add wrappers
    $wrapper_options = array('div' => 'Div', 'span' => 'Span', 'section' => 'Section', 'article' => 'Article', 'header' => 'Header', 'footer' => 'Footer', 'aside' => 'Aside');
    $form['region_wrapper'] = array(
      '#group' => 'additional_settings',
      '#type' => 'details',
      '#title' => t('Custom wrappers'),
      '#description' => t('Choose a wrapper. All Display Suite layouts support this option.')
    );

    // Hide the fieldset in case of the reset layout.
    if ($layout['layout'] === 'ds_reset') {
      $form['region_wrapper']['#access'] = FALSE;
    }

    foreach (array_keys($layout['regions']) as $region) {
      $form['region_wrapper'][$region] = array(
        '#type' => 'select',
        '#options' => $wrapper_options,
        '#title' => t('Wrapper for @region', array('@region' => $layout['regions'][$region])),
        '#default_value' => isset($layout['settings']['wrappers'], $layout['settings']['wrappers'][$region]) ? $layout['settings']['wrappers'][$region] : 'div',
      );
    }

    $form['region_wrapper']['layout_wrapper'] = array(
      '#type' => 'select',
      '#options' => $wrapper_options,
      '#title' => t('Layout wrapper'),
      '#default_value' => isset($layout->settings['layout_wrapper']) ? $layout['settings']['layout_wrapper'] : 'div',
      '#weight' => 10,
    );

    $form['region_wrapper']['layout_attributes'] = array(
      '#type' => 'textfield',
      '#title' => t('Layout attributes'),
      '#description' => 'E.g. role="navigation"',
      '#default_value' => isset($layout->settings['layout_attributes']) ? $layout['settings']['layout_attributes'] : '',
      '#weight' => 11,
    );

    $form['region_wrapper']['layout_attributes_merge'] = array(
      '#type' => 'checkbox',
      '#title' => t('Merge other layout attributes'),
      '#description' => 'Certain modules might provide additional attributes for the template wrapper, e.g. RDFa. Disable this option if you prefer to specify these attributes above.',
      '#default_value' => isset($layout->settings['layout_attributes_merge']) ? $layout['settings']['layout_attributes_merge'] : variable_get('ds_layout_attributes_merge', TRUE),
      '#weight' => 12,
    );

    // Add extra classes for the regions to have more control while theming.
    $form['ds_classes'] = array(
      '#group' => 'additional_settings',
      '#type' => 'details',
      '#title' => t('Custom classes'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#access' => empty($chosen_layout['flexible']),
    );

    $classes_access = (user_access('admin_classes') && module_exists('ds_ui'));
    $classes = _ds_classes();
    if (!empty($classes)) {

      $form['ds_classes']['layout_class'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $classes,
        '#title' => t('Class for layout'),
        '#default_value' => isset($layout->settings['classes']['layout_class']) ? $layout['settings']['classes']['layout_class'] : '',
      );

      foreach (array_keys($layout['regions']) as $region) {
        $form['ds_classes'][$region] = array(
          '#type' => 'select',
          '#multiple' => TRUE,
          '#options' => $classes,
          '#title' => t('Class for @region', array('@region' => $region)),
          '#default_value' => isset($layout['settings']['classes'][$region]) ? $layout['settings']['classes'][$region] : '',
        );
      }
      if ($classes_access) {
        $form['ds_classes']['info'] = array('#markup' => l(t('Manage region  and field CSS classes'), 'admin/structure/ds/classes', array('query' => drupal_get_destination())));
      }
    }
    else {
      if ($classes_access) {
        $form['ds_classes']['info'] = array('#markup' => '<p>' . t('You have not defined any CSS classes which can be used on regions.') . '</p><p>' . l(t('Manage region and field CSS classes'), 'admin/structure/ds/classes', array('query' => drupal_get_destination())) . '</p>');
      }
      else {
        $form['ds_classes']['#access'] = FALSE;
      }
    }
  }
  else {

    if ($view_mode != 'form') {
      // See if we can clone from another view mode.
      $options = array();
      $ds_layout_settings = config_get_storage_names_with_prefix('ds.layout_settings.' . $entity_type . '.' . $bundle);
      foreach ($ds_layout_settings as $config) {
        $row = config($config)->get();
        // Do not clone from form layouts.
        if ($row['view_mode'] == 'form') {
          continue;
        }
        if ($row['entity_type'] == $entity_type && $row['bundle'] == $bundle) {
          $options[$row['id']] = drupal_ucfirst(str_replace('_', ' ', $row['entity_type'])) . ' > ' . drupal_ucfirst(str_replace('_', ' ', $row['bundle'])) . ' > ' . drupal_ucfirst(str_replace('_', ' ', $row['view_mode']));
        }
      }

      if (!empty($options)) {

        // Clone from another layout.
        $form['ds_clone'] = array(
          '#type' => 'details',
          '#group' => 'additional_settings',
          '#title' => t('Clone layout'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form['ds_clone']['clone'] = array(
          '#title' => t('Select an existing layout to clone.'),
          '#type' => 'select',
          '#options' => $options,
          '#weight' => 20,
        );
        $form['ds_clone']['clone_submit'] = array(
          '#type' => 'submit',
          '#value' => t('Clone layout'),
          '#submit' => array('ds_field_ui_layout_clone'),
          '#weight' => 21,
        );
      }
    }
  }

  $form['ds_layouts']['id'] = array(
    '#type' => 'value',
    '#value' => isset($layout['id']) ? $layout['id'] : $form['#export_id'],
  );

  $form['ds_layouts']['old_layout'] = array(
    '#type' => 'value',
    '#value' => isset($layout['layout']) ? $layout['layout'] : 0,
  );

  // Add validate and submit handlers. Layout needs be first so
  // we can reset the type key for Field API fields.
  $form['#validate'][] = 'ds_field_ui_layouts_validate';
  $submit = $form['#submit'];
  $form['#submit'] = array('ds_field_ui_layouts_save');
  $form['#submit'] = array_merge($form['#submit'], $submit);
}

/**
 * Add the layout previews to the Field UI screen.
 *
 * @param $form
 *   A collection of form properties.
 * @param $form_state
 *   The state of the form
 * @param $ds_layouts
 *   Collection of all the layouts
 * @param $layout
 *   Current selected layout
 * @param $entity_type
 *   The name of the entity type.
 * @param $bundle
 *   The name of the bundle
 * @param $view_mode
 *   The name of the view_mode
 */
function _ds_field_ui_table_layouts_preview(&$form, &$form_state, $ds_layouts, $layout, $entity_type, $bundle, $view_mode) {
  $layout_string = '';

  $form['ds_layouts']['layout'] = array(
    '#type' => 'select',
    '#title' => t('Select a layout'),
    '#options' => $layout['layout_options'],
    '#default_value' => isset($layout['layout']) ? $layout['layout'] : '',
    '#prefix' => '<div class="ds-select-layout">',
    '#suffix' => '</div>',
    '#weight' => -1,
    '#ajax' => array(
      'callback' => 'ds_field_ui_table_layouts_preview_callback',
      'wrapper' => 'ds_layout_wrapper',
    ),
  );

  $form['ds_layouts']['preview'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="ds_layout_wrapper">',
    '#suffix' => '</div>',
    '#weight' => -3,
  );

  if (isset($layout['layout']) || isset($form_state['values']['layout'])) {
    $layout_string = isset($form_state['values']['layout']) ? $form_state['values']['layout'] : $layout['layout'];
  }

  if (!empty($layout_string)) {
    $chosen_layout = $ds_layouts[$layout_string];

    if (empty($chosen_layout['flexible'])) {

      $selected = '<strong>' . $chosen_layout['label'] . '</strong>';
      $selected .= '<br/>' . t('The default template can be found in %path', array('%path' => $chosen_layout['path']));
      $suggestions = t('Template suggestions') . ':<ul>';
      $suggestions_array = array();

      $suggestions_array[0] = $layout_string . '--' . $entity_type;
      $suggestions_array[2] = $layout_string . '--' . $entity_type . '-' . $bundle;
      if (!isset($form_state['no_view_mode_suggestions']) && $view_mode != 'default') {
        $suggestions_array[1] = $layout_string . '--' . $entity_type . '-' . $view_mode;
        $suggestions_array[3] = $layout_string . '--' . $entity_type . '-' . $bundle . '-' . $view_mode;
      }

      ksort($suggestions_array);
      $suggestions = '<ul><li>' . implode('.tpl.php</li><li>', $suggestions_array) . '.tpl.php</li></ul>';
    }
    else {
      $suggestions = '';
      $selected = t('You have selected the flexible %layout_label layout.', array('%layout_label' => $chosen_layout['label'], '%path' => $chosen_layout['path']));
    }

    if (isset($form_state['values']['layout']) || (!empty($layout) && isset($layout['regions']))) {
      $fallback_image = drupal_get_path('module', 'ds') . '/images/preview.png';
      $current_layout = isset($form_state['values']['layout']) && (!isset($layout->layout) || $form_state['values']['layout'] != $layout->layout) ? t('Current layout (after save)') : t('Current layout');
      $image = (isset($chosen_layout['image']) && !empty($chosen_layout['image'])) ? $chosen_layout['path'] . '/' . $layout_string . '.png' : $fallback_image;

      $form['ds_layouts']['preview'] ['title'] = array(
        '#markup' => '<div class="ds-layout-preview-title">' . $current_layout . '</div>',
      );
      $form['ds_layouts']['preview'] ['image'] = array(
        '#markup' => '<div class="ds-layout-preview-image"><img src="' . base_path() . $image . '"/></div>',
      );
      $form['ds_layouts']['preview'] ['suggestions'] = array(
        '#markup' => '<div class="ds-layout-preview-suggestion"><p>' . $selected . '</p><p>' . t('!suggestions', array('!suggestions' => strtr($suggestions, '_', '-'))) . '</p></div>',
      );
    }

    if (isset($form_state['values']['layout']) && (!isset($layout['layout']) || $form_state['values']['layout'] != $layout['layout'])) {

      // Get admin path.
      $admin_path = field_ui_bundle_admin_path($entity_type, $bundle);
      if ($view_mode != 'form') {
        $admin_path .= '/display';
      }
      else {
        $admin_path .= '/fields';
      }

      // If regions aren't set we don't have to move fields.
      if (isset($layout['regions'])) {
        $url = 'admin/structure/ds/change-layout/' . $entity_type . '/' . $bundle . '/' . $view_mode . '/' . $layout_string . '?destination=' . $admin_path;
        $form['#validate'][] = 'ds_field_ui_change_layout_validate';
      }
      else {
        $url = $admin_path;
      }

      if ($view_mode != 'default' && $view_mode != 'form') {
        $url .= '/' . $view_mode;
      }

      $form['layout_changed_url'] = array(
        '#type' => 'value',
        '#value' => $url,
      );

      $form['#submit'][] = 'ds_field_ui_change_layout_submit';
    }
  }
}

/**
 * Ajax callback for _ds_field_ui_table_layouts_preview().
 */
function ds_field_ui_table_layouts_preview_callback($form, $form_state) {
  return $form['ds_layouts']['preview'];
}

/**
 * Form validation handler for _ds_field_ui_table_layouts_preview().
 */
function ds_field_ui_change_layout_validate(&$form, &$form_state) {
  $key1 = array_search('ds_field_ui_layouts_save', $form['#submit']);
  $key2 = array_search('ds_field_ui_fields_save', $form['#submit']);
  unset($form['#submit'][$key1]);
  unset($form['#submit'][$key2]);
}

/**
 * Form submission handler for _ds_field_ui_table_layouts_preview().
 */
function ds_field_ui_change_layout_submit($form, &$form_state) {
  unset($_GET['destination']);
  global $base_url;
  $url = $base_url . '/' . $form_state['values']['layout_changed_url'];
  $form_state['redirect'] = $url;
}

/**
 * Add the fields to the Field UI form.
 *
 * @param $entity_type
 *   The name of the entity type.
 * @param $bundle
 *   The name of the bundle
 * @param $view_mode
 *   The name of the view_mode
 * @param $form
 *   A collection of form properties.
 * @param $form_state
 *   A collection of form_state properties.
 */
function _ds_field_ui_fields($entity_type, $bundle, $view_mode, &$form, &$form_state) {

  // Do not add the fields if there is no layout.
  if (!isset($form['#ds_layout'])) {
    return;
  }

  // Get the fields and put them on the form.
  $fields = ds_get_fields($entity_type, FALSE);

  // Ultimate alter on Field UI fields, only used for edge cases.
  $context = array(
    'entity_type' => $entity_type,
    'bundle' => $bundle,
    'view_mode' => $view_mode,
  );

  drupal_alter('ds_fields_ui', $fields, $context);

  // Get field settings.
  $field_settings = ds_get_field_settings($entity_type, $bundle, $view_mode, FALSE);
  $form['#field_settings'] = $field_settings;

  $table = &$form['fields'];
  $form['#ds_fields'] = array();

  $field_label_options = array(
    'above' => t('Above'),
    'inline' => t('Inline'),
    'hidden' => t('<Hidden>'),
  );
  drupal_alter('ds_label_options', $field_label_options);

  // Regions for fields.
  $field_regions = array();
  if (isset($form['#ds_layout']['settings']['fields'])) {
    $field_regions = $form['#ds_layout']['settings']['fields'];
  }

  foreach ($fields as $key => $field) {

    // Check on ui_limit.
    if (isset($field['ui_limit'])) {

      $continue = TRUE;
      foreach ($field['ui_limit'] as $limitation) {
        list($limit_bundle, $limit_view_mode) = explode('|', $limitation);
        if ($limit_bundle == '*' || $limit_bundle == $bundle) {
          if ($limit_view_mode == '*' || $limit_view_mode == $view_mode) {
            $continue = FALSE;
          }
        }
      }

      if ($continue) {
        continue;
      }
    }

    $form['#ds_fields'][] = $key;

    // Check on formatter settings.
    if (isset($form_state['formatter_settings'][$key])) {
      $field['formatter_settings'] = $form_state['formatter_settings'][$key];
    }
    elseif (isset($field_settings[$key]['formatter_settings'])) {
      $field['formatter_settings'] = $field_settings[$key]['formatter_settings'];
      $form_state['formatter_settings'][$key] = $field['formatter_settings'];
    }

    if (!isset($field_settings[$key]['ft']) && isset($field_settings[$key]['ft'])) {
      $form_state['formatter_settings'][$key]['ft'] = $field_settings[$key]['ft'];
    }

    $value = isset($form_state['formatter_settings']) ? $form_state['formatter_settings'] : array();

    $hidden = array('hidden' => t('<Hidden>'));

    // Get the formatters from the field instance
    $field_instance = drupal_container()->get('plugin.manager.ds')->createInstance($field['plugin_id']);
    $formatters = $field_instance->formatters();

    // This should be temporary. Don't want to copy stuff from the object to
    // the field each ajax refresh.
    if (!empty($formatters)) {
      $formatters = $hidden + $formatters;
    }
    else {
      $formatters = $hidden + array('default' => t('Default'));
    }

    $field_instance_settings = $field_instance->settings();
    $field_instance_default_settings = $field_instance->defaultSettings();
    if (!empty($field_instance_settings)) {
      $field['properties']['settings'] = $field_instance_settings;
    }
    if (!empty($field_instance_default_settings)) {
      $field['properties']['default'] = $field_instance_default_settings;
    }

    $table[$key] = array(
      '#row_type' => 'field',
      '#js_settings' => array('field'),
      '#region_callback' => 'field_ui_display_overview_row_region',
      '#attributes' => array('class' => array('draggable', 'tabledrag-leaf')),
      'human_name' => array(
        '#markup' => check_plain($field['title']),
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => isset($field_settings[$key]['weight']) ? $field_settings[$key]['weight'] : 0,
        '#size' => 3,
        '#attributes' => array('class' => array('field-weight')),
      ),
      'parent_wrapper' => array(
        'parent' => array(
          '#type' => 'select',
          '#empty_value' => '',
          '#options' => array(),
          '#attributes' => array('class' => array('field-parent')),
          '#parents' => array('fields', $key, 'parent'),
        ),
        'hidden_name' => array(
          '#type' => 'hidden',
          '#default_value' => $key,
          '#attributes' => array('class' => array('field-name')),
        ),
      ),
      'label' => array(
        '#type' => 'select',
        '#options' => $field_label_options,
        '#default_value' => isset($field_settings[$key]['label']) ? $field_settings[$key]['label'] : 'hidden',
      ),
      'format' => array(
        'type' => array(
          '#type' => 'select',
          '#options' => $formatters,
          '#default_value' => isset($field_settings[$key]['format']) ? $field_settings[$key]['format'] : 'hidden',
          '#attributes' => array('class' => array('field-formatter-type')),
        ),
      ),
      'settings_summary' => array(),
      'settings_edit' => array(),
    );

    // Don't show summary or cogwheel in hidden region.
    if (_ds_field_ui_check_hidden_region($key, $form_state, $field_regions)) {
      continue;
    }

    $field['name'] = $key;
    $field['entity_type'] = $entity_type;
    $field['bundle'] = $bundle;
    $field['view_mode'] = $view_mode;

    if ($form_state['formatter_settings_edit'] == $key) {
      $table[$key]['settings_summary']['#attributes']['colspan'] = 2;
      $settings_form = ds_field_settings_form($field, $form_state);
      ds_field_row_form_format_construct($table, $key, $settings_form, $form_state);
    }
    else {
      // After saving, the settings are updated here as well. First we create
      // the element for the table cell.
      $summary = ds_field_settings_summary($field, $form_state);

      if (isset($summary)) {
        $table[$key]['settings_summary'] = $summary;
        ds_field_row_form_format_summary_construct($table, $key, $form_state);
      }
    }
  }

  // Add fields submit handler.
  $form['#submit'][] = 'ds_field_ui_fields_save';
}

/**
 * Alter the core field on the the Field UI form.
 *
 * @param $entity_type
 *   The name of the entity type.
 * @param $bundle
 *   The name of the bundle
 * @param $view_mode
 *   The name of the view_mode
 * @param $form
 *   A collection of form properties.
 * @param $form_state
 *   A collection of form_state properties.
 */
function _ds_field_ui_core_fields($entity_type, $bundle, $view_mode, &$form, &$form_state) {
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];
  $entity_display = entity_get_display($entity_type, $bundle, $view_mode);

  // Gather type information.
  $instances = field_info_instances($entity_type, $bundle);

  $field_types = field_info_field_types();
  $extra_fields = field_info_extra_fields($entity_type, $bundle, 'display');

  $table = &$form['fields'];

  // Regions for fields.
  $field_regions = array();
  if (isset($form['#ds_layout']['settings']['fields'])) {
    $field_regions = $form['#ds_layout']['settings']['fields'];
  }

  // Field rows.
  foreach ($instances as $key => $instance) {

    // Don't show summary or cogwheel in hidden region.
    if (_ds_field_ui_check_hidden_region($key, $form_state, $field_regions)) {
      $table[$key]['settings_summary']['#markup'] = '';
      $table[$key]['settings_edit'] = array();
      continue;
    }

    $field = field_info_field($instance['field_name']);
    $display = $instance['display'][$view_mode];

    // Check the currently selected formatter, and merge persisted values for
    // formatter settings.
    if (isset($form_state['values']['fields'][$key]['type'])) {
      $formatter_type = $form_state['values']['fields'][$key]['type'];
    }
    else {
      $formatter_type = $display['type'];
    }

    $settings = $entity_display->getComponent($key);
    if (isset($form_state['formatter_settings'][$key])) {
      $settings = array_merge($settings, $form_state['formatter_settings'][$key]);
    }
    //$settings += field_info_formatter_settings($formatter_type);

    // Import field settings and merge with Field API settings.
    if (!isset($form_state['formatter_settings'][$key]) && !empty($form['#field_settings'][$key]['formatter_settings']['ft'])) {
      $form_state['formatter_settings'][$key] = $settings;
      $form_state['formatter_settings'][$key]['ft'] = $form['#field_settings'][$key]['formatter_settings']['ft'];
    }

    // Change default value or Field API format, so we can change the right
    // settings form when clicking on the cogwheel.
    $form['fields'][$key]['format']['type']['#default_value'] = $formatter_type;

    $instance['display'][$view_mode]['type'] = $formatter_type;
    $formatter = field_info_formatter_types($formatter_type);
    //$instance['display'][$view_mode]['module'] = $formatter['module'];
    $instance['display'][$view_mode]['settings'] = $settings;

    // Base button element for the various formatter settings actions.
    $base_button = array(
      '#submit' => array(
        array($form_state['build_info']['callback_object'], 'multistepSubmit'),
      ),
      '#ajax' => array(
        'callback' => array($form_state['build_info']['callback_object'], 'multistepAjax'),
        'wrapper' => 'field-display-overview-wrapper',
        'effect' => 'fade',
      ),
      '#field_name' => $key,
    );

    if ($form_state['formatter_settings_edit'] == $key) {
      $formatter_type = $form_state['complete_form']['fields'][$key]['format']['type']['#default_value'];

      // Get the corresponding formatter object.
      if ($settings && $settings['type'] != 'hidden') {
        $formatter = drupal_container()->get('plugin.manager.field.formatter')->getInstance(array(
          'instance' => $instance,
          'view_mode' => $view_mode,
          'configuration' => $settings
        ));
      }
      else {
        $formatter = NULL;
      }


      // Add the default formatter settings if any.
      $settings_form = array();
      if ($formatter) {
        $settings_form = $formatter->settingsForm($form, $form_state);
      }

      // Add the field templates form when needed
      if (module_exists('ds_extras') && config('ds.extras')->get('field_template', FALSE)) {
        $context = array(
          'instance' => $instance,
          'view_mode' => $view_mode,
        );

        // Load the form
        module_load_include('inc', 'ds_extras', 'includes/admin');
        ds_extras_field_template_settings_form($settings_form, $form_state, $context);
      }

      // Allow other modules to alter the formatter settings form.
      // @todo check whether we can't make this cleaner, because we're
      // calling this twice now.
      $context = array(
        'formatter' => $formatter,
        'field' => $field,
        'instance' => $instance,
        'view_mode' => $view_mode,
        'form' => $form,
      );
      drupal_alter('field_formatter_settings_form', $settings_form, $form_state, $context);

      if ($settings_form) {
        $table[$key]['format']['#cell_attributes'] = array('colspan' => 3);
        $table[$key]['format']['settings_edit_form'] = array(
          '#type' => 'container',
          '#attributes' => array('class' => array('field-formatter-settings-edit-form')),
          '#parents' => array('fields', $key, 'settings_edit_form'),
          'label' => array(
            '#markup' => t('Format settings:'), // . ' <span class="formatter-name">' . $formatter['label'] . '</span>',
          ),
          'settings' => $settings_form,
          'actions' => array(
            '#type' => 'actions',
            'save_settings' => $base_button + array(
              '#type' => 'submit',
              '#name' => $key . '_formatter_settings_update',
              '#value' => t('Update'),
              '#op' => 'update',
            ),
            'cancel_settings' => $base_button + array(
              '#type' => 'submit',
              '#name' => $key . '_formatter_settings_cancel',
              '#value' => t('Cancel'),
              '#op' => 'cancel',
              // Do not check errors for the 'Cancel' button, but make sure we
              // get the value of the 'formatter type' select.
              '#limit_validation_errors' => array(array('fields', $key, 'type')),
            ),
          ),
        );
        $table[$key]['#attributes']['class'][] = 'field-formatter-settings-editing';
        $table[$key]['format']['type']['#attributes']['class'] = array('element-invisible');
      }
    }
    else {
      //$summary = module_invoke($formatter['module'], 'field_formatter_settings_summary', $field, $instance, $view_mode);

      // Allow other modules to alter the formatter summary.
      // @todo check whether we can't make this cleaner, because we're
      // calling this twice now.
      $context = array(
        'field' => $field,
        'instance' => $instance,
        'view_mode' => $view_mode,
      );
      drupal_alter('field_formatter_settings_summary', $summary, $context);

      if (module_exists('ds_extras') && config('ds.extras')->get('field_template', FALSE)) {
        module_load_include('inc', 'ds_extras', 'includes/admin');

        // Field template summary
        $functions = module_invoke_all('ds_field_theme_functions_info');
        $default_field_function = config('ds.extras')->get('ft-default');
        $field_function = isset($form_state['formatter_settings'][$key]['ft']['func']) ? $form_state['formatter_settings'][$key]['ft']['func'] : $default_field_function;

        if (!empty($summary)) {
          $summary .= '<br />';
        }
        $summary .= 'Field template: ' . check_plain($functions[$field_function]) . '<br />';
      }

      if (!empty($summary)) {
        $table[$key]['settings_summary'] = array();
        $table[$key]['settings_edit'] = array();

        $table[$key]['settings_summary'] = array(
          '#markup' => '<div class="field-formatter-summary">' . $summary . '</div>',
          '#cell_attributes' => array('class' => array('field-formatter-summary-cell')),
        );
        // Render the other part of the summary
        ds_field_row_form_format_summary_construct($table, $key, $form_state);
      }
    }
  }
}

/**
 * Helper function to check if we are in a hidden region or not.
 */
function _ds_field_ui_check_hidden_region($key, $form_state, $field_regions) {
  $continue = FALSE;
  if (isset($form_state['input']['fields'][$key]['region'])) {
    if ($form_state['input']['fields'][$key]['region'] == 'hidden') {
      $continue = TRUE;
    }
  }
  elseif (!isset($field_regions[$key]) || $field_regions[$key] == 'hidden') {
    $continue = TRUE;
  }

  return $continue;
}

/**
 * Helper function for building the formatter settings.
 */
function ds_field_row_form_format_construct(&$table, $key, $settings_form, $form_state) {

  $base_button = array(
    '#submit' => array(
      array($form_state['build_info']['callback_object'], 'multistepSubmit'),
    ),
    '#validate' => array(
      'ds_field_ui_fields_validate',
    ),
    '#ajax' => array(
      'callback' => array($form_state['build_info']['callback_object'], 'multistepAjax'),
      'wrapper' => 'field-display-overview-wrapper',
      'effect' => 'fade',
    ),
    '#field_name' => $key,
  );
  $table[$key]['format']['settings_edit'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('field-formatter-settings-edit-form')),
    '#parents' => array('fields', $key, 'settings_edit_form'),
    '#weight' => -5,
    // Create a settings form where hooks can pick in.
    'settings' => $settings_form,
    'actions' => array(
      '#type' => 'actions',
      'save_settings' => $base_button + array(
        '#type' => 'submit',
        '#name' => $key . '_formatter_settings_update',
        '#value' => t('Update'),
        '#op' => 'update',
      ),
      'cancel_settings' => $base_button + array(
        '#type' => 'submit',
        '#name' => $key . '_formatter_settings_cancel',
        '#value' => t('Cancel'),
        '#op' => 'cancel',
        // Do not check errors for the 'Cancel' button.
        '#limit_validation_errors' => array(),
      ),
    ),
  );
  $table[$key]['#attributes']['class'][] = 'field-formatter-settings-editing';
  $table[$key]['format']['type']['#attributes']['class'] = array('element-invisible');
}

/**
 * Helper function for formatter summary settings.
 */
function ds_field_row_form_format_summary_construct(&$table, $key, $form_state) {

  $base_button = array(
    '#submit' => array(
      array($form_state['build_info']['callback_object'], 'multistepSubmit'),
    ),
    '#ajax' => array(
      'callback' => array($form_state['build_info']['callback_object'], 'multistepAjax'),
      'wrapper' => 'field-display-overview-wrapper',
      'effect' => 'fade',
    ),
    '#field_name' => $key,
  );

  // Add the configure button.
  $table[$key]['settings_edit'] = $base_button + array(
    '#type' => 'image_button',
    '#name' => $key . '_formatter_settings_edit',
    '#src' => 'core/misc/configure-dark.png',
    '#attributes' => array('class' => array('field-formatter-settings-edit'), 'alt' => t('Edit')),
    '#op' => 'edit',
    // Do not check errors for the 'Edit' button.
    '#limit_validation_errors' => array(),
    '#prefix' => '<div class="field-formatter-settings-edit-wrapper">',
    '#suffix' => '</div>',
  );
}

/**
 * Utility function to check if we need to save anything for this field.
 */
function _ds_field_valid($key, $field, &$form_state, $view_mode = 'default') {
  $continue = FALSE;

  // Ignore the Field group module and the region to block plugin.
  if ($key == '_add_new_group' || $key == '_add_new_field' || $key == '_add_new_block_region') {
    $continue = TRUE;
  }

  // If the field is in hidden region, do not save. Check if the
  // field has a type key which means it's from Field API and
  // we need to reset that type to 'hidden' so it doesn't get
  // fired by Field API in the frontend.
  if (isset($field['region']) && $field['region'] == 'hidden') {
    if (isset($field['type']) && $view_mode != 'form') {
      $form_state['values']['fields'][$key]['type'] = 'hidden';
    }

    // In case of a form, fields will be set with #access to FALSE.
    if ($view_mode != 'form') {
      $continue = TRUE;
    }
  }

  return $continue;
}

/**
 * Utility function to return CSS classes.
 */
function _ds_classes($name = 'regions') {
  static $classes = array();

  if (!isset($classes[$name])) {
    $classes[$name] = array();
    $custom_classes = trim(config('ds.classes')->get($name));
    if (!empty($custom_classes)) {
      $classes[$name][''] = t('None');
      $custom_classes = explode("\n", $custom_classes);
      foreach ($custom_classes as $key => $value) {
        $classes_splitted = explode("|", $value);
        $key = trim($classes_splitted[0]);
        $friendly_name = isset($classes_splitted[1]) ? trim($classes_splitted[1]) : $key;
        $classes[$name][check_plain($key)] = $friendly_name;
      }
    }
  }

  return $classes[$name];
}

/**
 * Utility function to sort a multidimensional array by a value in a sub-array.
 *
 * @param $a
 *   The array to sort.
 * @param $subkey
 *   The subkey to sort by.
 */
function _ds_sort_fields($a, $subkey) {
  foreach ($a as $k => $v) {
    if (isset($v[$subkey])) {
      $b[$k] = $v[$subkey];
    }
  }
  asort($b);
  foreach ($b as $key => $val) {
    $c[$key] = $a[$key];
  }
  return $c;
}
