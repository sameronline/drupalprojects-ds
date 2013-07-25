<?php

/**
 * @file
 * Display Suite Extras administrative functions.
 */

use Drupal\Core\Extension\ModuleHandler;

/**
 * Menu callback: Display Suite extras settings.
 */
function ds_extras_settings($form) {

  $config = config('ds.extras');

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['fs1'] = array(
    '#type' => 'details',
    '#title' => t('Field Templates'),
    '#group' => 'additional_settings',
    '#tree' => TRUE,
  );

  $form['fs1']['field_template'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Field Templates'),
    '#description' => t('Customize the labels and the HTML output of your fields.'),
    '#default_value' => $config->get('field_template'),
  );

  $theme_functions = module_invoke_all('ds_field_theme_functions_info');
  $form['fs1']['ft-default'] = array(
    '#type' => 'select',
    '#title' => t('Default Field Template'),
    '#options' => $theme_functions,
    '#default_value' => $config->get('ft-default'),
    '#description' => t('Default will output the field as defined in Drupal Core.<br />Reset will strip all HTML.<br />Minimal adds a simple wrapper around the field.<br/>There is also an Expert Field Template that gives full control over the HTML, but can only be set per field.<br /><br />You can override this setting per field on the "Manage display" screens or when creating fields on the instance level.<br /><br /><strong>Template suggestions</strong><br />You can create .html.twig files as well for these field theme functions, e.g. field--reset.html.twig, field--minimal.html.twig<br /><br /><label>CSS classes</label>You can add custom CSS classes on the <a href="!url">classes form</a>. These classes can be added to fields using the Default Field Template.<br /><br /><label>Advanced</label>You can create your own custom field templates which need to be defined with hook_ds_field_theme_functions_info(). See ds.api.php for an example.', array('!url' => url('admin/structure/ds/classes'))),
    '#states' => array(
      'visible' => array(
        'input[name="additional_settings[fs1][field_template]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['fs1']['ft-kill-colon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide colon'),
    '#default_value' => $config->get('ft-kill-colon'),
    '#description' => t('Hide the colon on the reset field template.'),
    '#states' => array(
      'visible' => array(
        'select[name="additional_settings[fs1][ft-default]"]' => array('value' => 'theme_ds_field_reset'),
        'input[name="additional_settings[fs1][field_template]"]' => array('checked' => TRUE),
      ),
    ),
  );


  $form['fs2'] = array(
    '#type' => 'details',
    '#title' => t('Extra fields'),
    '#group' => 'additional_settings',
    '#tree' => TRUE,
  );

  $form['fs2']['fields_extra'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable extra fields'),
    '#description' => t('Make fields from other modules available on the "Manage display" screens.'),
    '#default_value' => $config->get('fields_extra'),
  );

  $form['fs2']['fields_extra_list'] = array(
    '#type' => 'textarea',
    '#description' => t('Enter fields line by line, where each line is a combination of entity type, bundle and field name. E.g. node|article|tweetbutton. It might be possible that the actual content of the field depends on configuration of that field/module.'),
    '#default_value' => $config->get('fields_extra_list'),
    '#states' => array(
      'visible' => array(
        'input[name="additional_settings[fs2][fields_extra]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['fs4'] = array(
    '#type' => 'details',
    '#title' => t('Other'),
    '#group' => 'additional_settings',
    '#tree' => TRUE,
  );

  $form['fs4']['switch_view_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('View mode per node'),
    '#description' => t('Change view modes for individual nodes. A new tab \'Display settings\' will appear on the content create form.<br />You can also pass the name of a view mode through the URL, eg node/x?v=full.<br />If you install the Page manager module and override the node view, Page manager will win.'),
    '#default_value' => $config->get('switch_view_mode'),
  );

  $form['fs4']['field_permissions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Field permissions'),
    '#description' => t('Enables view permissions on all Display Suite fields.'),
    '#default_value' => $config->get('field_permissions'),
  );

  /*if (module_exists('block')) {
    $form['fs4']['region_to_block'] = array(
      '#type' => 'checkbox',
      '#title' => t('Region to block'),
      '#description' => t('Create additional regions exposed as block. Note: this will not work on the default view mode.'),
      '#default_value' => $config->get('region_to_block'),
    );
  }*/

  if (module_exists('flag')) {
    $form['fs4']['flag'] = array(
      '#type' => 'checkbox',
      '#title' => t('Flag'),
      '#description' => t('Expose flags as fields on nodes.'),
      '#default_value' => $config->get('flag'),
    );
  }

  $form['fs4']['switch_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('View mode switcher'),
    '#description' => t('Adds a field with links to switch view modes inline with Ajax. Only works for nodes at this time.'),
    '#default_value' => $config->get('switch_field'),
  );

  $form['fs4']['hidden_region'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hidden region'),
    '#description' => t('Add a hidden region to the layouts. Fields will be built but not printed.'),
    '#default_value' => $config->get('hidden_region'),
  );

  $form['#validate'][] = 'ds_extras_settings_validate';
  $form['#submit'][] = 'ds_extras_settings_submit';
  $form['#attached']['js'][] = drupal_get_path('module', 'ds_extras') . '/js/ds_extras.admin.js';

  return $form;
}

/**
 * Validate callback: Extras settings screen.
 */
function ds_extras_settings_validate($form, &$form_state) {
  foreach ($form_state['values'] as $tab => $value) {
    if (is_array($value)) {
      foreach ($value as $variable => $val) {
        $form_state['values'][$variable] = $val;
      }
    }
    unset($form_state['values'][$tab]);
  }
  unset($form_state['values']['submit'], $form_state['values']['form_id'], $form_state['values']['form_token'], $form_state['values']['form_build_id'], $form_state['values']['op']);
}

/**
 * Submit callback: Extras settings screen.
 */
function ds_extras_settings_submit($form, &$form_state) {

  foreach ($form_state['values'] as $key => $value) {
    config('ds.extras')->set($key, $value)->save();
  }

  // @todo create ds cache clear api function ?
  //cache()->deletePrefix('ds_fields');
  entity_info_cache_clear();
  field_info_cache_clear();
  drupal_container()->get('module_handler')->resetImplementations();
  drupal_theme_rebuild();
  menu_router_rebuild();
}

/**
 * Alter Manage display screen.
 */
function ds_extras_field_ui_alter(&$form, &$form_state) {
  // Attach js.
  $form['#attached']['js'][] = drupal_get_path('module', 'ds_extras') . '/js/ds_extras.admin.js';

  // Region to block only fires if there is a layout and we're working on the
  // a view mode which is not equal to default.
  if (isset($form['#ds_layout']) && $form['#mode'] != 'default' && config('ds.extras')->get('region_to_block', FALSE)) {

    $layout = $form['#ds_layout'];

    // Get the entity_type, bundle and view mode.
    $entity_type = $form['#entity_type'];
    $bundle = $form['#bundle'];
    $view_mode = $form['#mode'];

    $region_blocks_options = array();
    $region_blocks = config('ds.extras')->get('region_blocks');
    foreach ($region_blocks as $key => $block) {
      if ($block['info'] == "{$entity_type}_{$bundle}_{$view_mode}") {
        $region_blocks_options[$key] = t('Remove') . ' ' . $block['title'];
      }
    }

    $form['region_to_block'] = array(
      '#type' => 'details',
      '#group' => 'additional_settings',
      '#title' => t('Block regions'),
      '#description' => t('Create additional regions in this layout which will be exposed as blocks. Note that preprocess fields will fail to print.')
    );

    $form['region_to_block']['new_block_region'] = array(
      '#type' => 'textfield',
      '#title' => t('Region name'),
      '#description' => t('Enter a name to create a new region.'),
    );
    $form['region_to_block']['new_block_region_key'] = array(
      '#title' => t('Machine name'),
      '#type' => 'machine_name',
      '#default_value' => '',
      '#maxlength' => 32,
      '#required' => FALSE,
      '#description' => t('The machine-readable name of this block region. This name must contain only lowercase letters and underscores. This name must be unique.'),
      '#disabled' => FALSE,
      '#machine_name' => array(
        'exists' => 'ds_extras_region_to_block_unique',
        'source' => array('region_to_block', 'new_block_region'),
      ),
    );

    if (!empty($region_blocks_options)) {
      $form['region_to_block']['remove_block_region'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Existing block regions'),
        '#options' => $region_blocks_options,
        '#description' => t('Check the regions you want to remove.'),
      );
    }

    $form['#submit'][] = 'ds_extras_block_submit';
  }
}

/**
 * Implements hook_ds_field_format_summary().
 */
function ds_extras_ds_field_format_summary($field) {
  if (isset($field['plugin_settings'])) {
    foreach ($field['plugin_settings'] as $key => $value) {
      if (!empty($value)) {
        return t('Configured');
        break;
      }
    }
  }
  return t('Not configured');
}

/**
 * Implements hook_ds_field_settings_form().
 */
function ds_extras_ds_field_settings_form($field) {
  $form = array();

  // Switch field.
  if (config('ds.extras')->get('switch_field') && $field['name'] == 'ds_switch_field') {
    $entity_type = $field['entity_type'];
    $bundle = $field['bundle'];
    $view_mode = $field['view_mode'];
    $settings = isset($field['plugin_settings']['vms']) ? $field['plugin_settings']['vms'] : array();
    $view_modes = entity_get_view_modes($entity_type);

    $form['info'] = array(
      '#markup' => t('Enter a label for the link for the view modes you want to switch to.<br />Leave empty to hide link. They will be localized.'),
    );

    foreach ($view_modes as $key => $value) {

      $view_mode_settings = field_view_mode_settings($entity_type, $bundle);
      $visible = !empty($view_mode_settings[$key]['custom_settings']);

      if ($visible) {
        $form['vms'][$key] = array(
          '#type' => 'textfield',
          '#default_value' => isset($settings[$key]) ? $settings[$key] : '',
          '#size' => 20,
          '#title' => check_plain($value['label']),
        );
      }
    }
  }

  return $form;
}

/**
 * Submit callback after Field UI submission of a views display.
 */
function ds_extras_vd_field_ui_submit($form, &$form_state) {
  // Add the 'type' key to the extra title key so we can ditch the notice.
  $form_state['values']['fields']['title']['type'] = 'hidden';
}

/**
 * Submit callback: manage block regions.
 */
function ds_extras_block_submit($form, &$form_state) {

  // Create new region.
  if (!empty($form_state['values']['region_to_block']['new_block_region'])) {

    // Get the entity_type, bundle and view mode.
    $entity_type = $form['#entity_type'];
    $bundle = $form['#bundle'];
    $view_mode = $form['#mode'];

    $block = array(
      'title' => $form_state['values']['region_to_block']['new_block_region'],
      'info' => "{$entity_type}_{$bundle}_{$view_mode}",
    );

    $block_key = $form_state['values']['region_to_block']['new_block_region_key'];
    $region_blocks = config('ds.extras')->get('blocks');
    if (empty($region_blocks)) {
      $region_blocks = array();
    }
    $region_blocks[$block_key] = $block;
    config('ds.extras')->set('region_blocks', $region_blocks)->save();
  }

  // Remove a region.
  if (isset($form_state['values']['region_to_block']['remove_block_region'])) {
    $save = FALSE;
    $region_blocks = config('ds.extras')->get('blocks');
    $remove = $form_state['values']['region_to_block']['remove_block_region'];
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
      config('ds.extras')->set('region_blocks', $region_blocks)->save();
    }
  }
}

/**
 * Return unique region to block.
 */
function ds_extras_region_to_block_unique($name) {
  $region_blocks = config('ds.extras')->get('region_blocks');
  if (empty($region_blocks)) {
    $region_blocks = array();
  }
  $value = strtr($name, array('-' => '_'));
  return isset($region_blocks[$value]) ? TRUE : FALSE;
}
