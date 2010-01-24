<?php
// $Id$

/**
 * @file
 * Tools for Display suite like export & import.
 */

/**
 * Import functionality.
 */
function ds_import($form_state) {
  $form = array();

  $form['import'] = array(
    '#title' => t('Import data'),
    '#description' => t('Paste data to import field settings. Do not include the function declaration from the export. <strong>Warning: existing data will be overwritten!</strong>'),
    '#type' => 'textarea',
    '#cols' => 60,
    '#default_value' => '',
    '#required' => TRUE,
    '#rows' => 10,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Import submit function.
 */
function ds_import_submit($form_id, &$form_state) {
  $error = FALSE;

  ob_start();
  eval($form_state['values']['import']);
  ob_end_clean();

  // Validate the data. $data variable comes from the eval() function above.
  if (!is_array($data)) {
    $error = TRUE;
    drupal_set_message(t('Data string is not an array.'), 'error');
  }

  // All ok, let's import.
  if ($error == FALSE) {
    ds_import_data($data, TRUE, TRUE);
  }
}

/**
 * Import data function.
 *
 * @param array $data A complete data array with settings.
 * @param boolean $dsm Whether to output drupal messages.
 * @param boolean $override Whether to override existing settings or not.
 * @param string $revert_module The module to revert.
 * @param string $revert_type The object type to revert.
 * @param string $revert_build_mode The build mode to revert.
 */
function ds_import_data($data, $dsm = TRUE, $override = TRUE, $revert_module = '', $revert_type = '', $revert_build_mode = '') {
  foreach ($data as $module => $value) {
    foreach ($value as $type => $settings) {
      if ($dsm) {
        drupal_set_message(t('Saved display settings for @module and @type', array('@module' => $module, '@type' => $type)));
      }
      $old_settings = variable_get($module .'_display_settings_'. $type, array());
      foreach ($settings as $build_mode => $setting) {
        // Override no matter what. This comes from the import settings screen.
        if ($override) {
          $settings[$build_mode]['status'] = DS_SETTINGS_UI;
        }
        // We are importing from hook_ds_default_settings(). We need to
        // a) check if we have old settings and keep them if they are overridden.
        // b) compare $revert_module, $revert_type and $revert_build_mode and revert if necessary.
        else {
          if (isset($old_settings[$build_mode]['status']) && $old_settings[$build_mode]['status'] == DS_SETTINGS_OVERRIDDEN) {
            if ($module == $revert_module && $type == $revert_type && $build_mode == $revert_build_mode) {
              $settings[$build_mode]['status'] = DS_SETTINGS_DEFAULT;
            }
            else {
              $settings[$build_mode] = $old_settings[$build_mode];
            }
          }
          else {
            $settings[$build_mode]['status'] = DS_SETTINGS_DEFAULT;
          }
        }
      }
      // Keep old settings which aren't in the imported ones.
      if (!empty($old_settings)) {
        foreach ($old_settings as $build_mode => $setting) {
          if (!isset($settings[$build_mode])) {
            $settings[$build_mode] = $old_settings[$build_mode];
          }
        }
      }
      variable_set($module .'_display_settings_'. $type, $settings);
    }
  }
}

/**
 * Export functionality.
 */
function ds_export(&$form_state) {
  $form = array();
  $options = array();
  $step = isset($form_state['storage']['step']) ? $form_state['storage']['step'] + 1 : 1;

  switch ($step) {

    // Show selection form.
    case 1:
      foreach (module_implements('ds_api') as $module) {
        $api_info = ds_api_info($module);
        $module = $api_info['module'];
        foreach ($api_info['types']() as $tkey => $type) {
          $options[$module .'-'. $tkey] = $type->name;
        }
      }

      if (!empty($options)) {
        $form['#prefix'] = t('Select one or more types to export.');
        $form['types'] = array(
          '#title' => 'Types',
          '#type' => 'checkboxes',
          '#options' => $options,
          '#required' => TRUE,
        );
        $form['module'] = array(
          '#title' => t('Module'),
          '#type' => 'textfield',
          '#description' => t('Fill in a name for your module (optional)'),
        );
      }
      else {
        $form['info'] = array(
          '#type' => 'item',
          '#value' => t('No object types found to export.'),
        );
      }
      break;

    // Show export.
    case 2:
      $export = array();
      foreach ($form_state['values']['types'] as $otype => $value) {
        if ($otype === $value) {
          list($module, $type) = explode('-', $value);
          $settings = variable_get($module .'_display_settings_'. $type, array());
          if (!empty($settings)) {
            $export[$module][$type] = $settings;
          }
        }
      }

      if (!empty($export)) {
        $module_name = (!empty($form_state['values']['module'])) ? $form_state['values']['module'] : 'YOURMODULENAME';
        $export_value = ds_var_export($export);
        $form['export_hook'] = array(
          '#title' => t('Display suite hook function'),
          '#description' => t('This is the hook you need to implement for default data. You are free to return all data in this hook, this is just a suggestion.'),
          '#value' => "<?php\n/**\n * Implementation of hook_ds_default_settings().\n */\nfunction ". $module_name ."_ds_default_settings() {\n  include_once('". $module_name .".ds_default.inc');\n  return _". $module_name ."_ds_default_settings();\n}\n",
          '#type' => 'textarea',
          '#cols' => 60,
          '#rows' => 8,
        );
        $form['export_settings'] = array(
          '#title' => t('Display suite export data'),
          '#description' => t('You can paste this data into @module_name/@module_name.ds_default.inc. If you only want to store this data to import later, omit the function declaration.', array('@module_name' => $module_name)),
          '#value' => "<?php\n/**\n * @file\n * Display suite default settings.\n */\nfunction _". $module_name ."_ds_default_settings() {\n  \$data = ". $export_value .";\n  return \$data;\n}",
          '#type' => 'textarea',
          '#cols' => 60,
          '#rows' => 15,
        );
      }
      else {
        $form['info'] = array(
          '#type' => 'item',
          '#value' => t('No settings found to export.'),
        );
      }

      break;
  }

  if ($step == 1 && !empty($options)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Export'),
    );
  }

  $form['step'] = array(
    '#type' => 'value',
    '#value' => $step,
  );

  return $form;
}

/**
 * Export submit function.
 */
function ds_export_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['storage']['step'] = $form_state['values']['step'];
}

/**
 * Revert form.
 *
 * @param string $module The name of the module.
 * @param string $object_type The object type name.
 * @param string $build_mode The build mode.
 */
function ds_revert_form($form_state, $module = '', $type = '', $build_mode = '') {

  $display_settings = variable_get($module .'_display_settings_'. $type, array());

  if (isset($display_settings[$build_mode]) &&
    isset($display_settings[$build_mode]['status']) &&
    $display_settings[$build_mode]['status'] == DS_SETTINGS_OVERRIDDEN
    ) {

    $question = t('Are you sure you want to revert the display settings for %module, %type and %build_mode. This will bring back the default settings and any changes you have done will be lost.', array('%module' => $module, '%type' => $type, '%build_mode' => $build_mode));
    $path = 'admin/ds/layout';
    $form['#module'] = $module;
    $form['#object_type'] = $type;
    $form['#build_mode'] = $build_mode;
    $form['#path'] = $path;
    return confirm_form($form, $question, $path);
  }
  else {
    drupal_set_message(t('Variables not found.'));
    drupal_goto('admin/ds');
  }
}

/**
 * Revert form submit.
 */
function ds_revert_form_submit($form, &$form_state) {
  drupal_set_message(t('Display settings for %module, %type and %build_mode have been reverted.', array('%module' => $form['#module'], '%type' => $form['#object_type'], '%build_mode' => $form['#build_mode'])));
  ds_import_default_data($form['#module'], $form['#object_type'], $form['#build_mode']);
  $form_state['redirect'] = $form['#path'];
}

/**
 * Nice var exporter, based on Views.
 */
function ds_var_export($var, $prefix = '  ', $init = TRUE, $indent = '  ') {
  if (is_array($var)) {
    if (empty($var)) {
      $output = 'array()';
    }
    else {
      $prefix .= '  ';
      $old = $indent;
      $indent .= '  ';
      $output = "array(\n";
      foreach ($var as $key => $value) {
        $output .= "$indent'$key' => " . ds_var_export($value, $prefix, FALSE, $indent) . ",\n";
      }
      $indent = $old;
      $output .= "$indent)";
    }
  }
  else if (is_bool($var)) {
    $output = $var ? 'TRUE' : 'FALSE';
  }
  else if (is_string($var) && strpos($var, "\n") !== FALSE) {
    // Replace line breaks in strings with a token for replacement
    // at the very end. This protects multi-line strings from
    // unintentional indentation.
    $var = str_replace("\n", "***BREAK***", $var);
    $output = var_export($var, TRUE);
  }
  else {
    $output = var_export($var, TRUE);
  }


  if ($init) {
    $output = str_replace("***BREAK***", "\n", $output);
  }

  return $output;
}
