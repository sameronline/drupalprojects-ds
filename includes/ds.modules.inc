<?php

/**
 * @file
 * Info about a module.
 */

/**
 * Provide information about a module to the admin interface.
 */
function ds_info($module) {
  $info = '';

  // API info about this module.
  $api_info = ds_api_info($module);
  $help = isset($api_info['help']) ? $api_info['help'] : array();

  // Let other modules add info.
  drupal_alter('ds_info', $help, $module);

  // General info (if any).
  if (!empty($help)) {
    $info = theme('item_list', $help);
  }

  if (!empty($info)) {
    return $info;
  }
  else {
    return t('No general info found.');
  }
}

/**
 * Provide information about all installed modules
 */
function ds_module_list() {
  $output = '';

  $modules = ds_get_display_handlers();
  foreach ($modules as $key => $module) {
    $form = array();

    // Fieldset
    $form[$key] = array(
      '#type' => 'fieldset',
      '#title' => t($module['name']),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#access' => user_access('administer '. $key),
    );

    // Tools
    $fields = l('Fields', DS_PATH_MODULES .'/'.  $key .'/fields');
    $fieldgroups = l('Fieldgroups', DS_PATH_MODULES .'/'.  $key .'/fieldgroups');
    $buildmodes = l('Build modes', DS_PATH_MODULES .'/'.  $key .'/buildmodes');
    $plugins = l('Plugins', DS_PATH_MODULES .'/'.  $key .'/plugins');

    $form[$key]['tools'] = array(
      '#value' => '<p>'. $fields .' | '. $fieldgroups .' | '. $buildmodes .' | '. $plugins .'</p>',
      '#weight' => -1,
    );

    // Help
    $form[$key]['help'] = array(
      '#value' => $module['help'],
    );
    
    $output .= drupal_render($form);
  }
  if (empty($output)) {
    $output = '<p>'. t('No modules which provide settings were found.') .'</p></p>'. t('For example, to change display options for nodes, use <a href="http://drupal.org/project/nd">Node Displays</a>. Other modules are listed in the <a href="http://drupal.org/node/644662">documentation</a>.') .'</p>';
  }

  return $output;
}