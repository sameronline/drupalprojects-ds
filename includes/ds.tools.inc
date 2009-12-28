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

  // Validate the data.
  if (!is_array($data)) {
    $error = TRUE;
    drupal_set_message(t('Data string is not an array.'), 'error');
  }

  // All ok, let's import.
  if ($error == FALSE) {
    $data = $data;
    foreach ($data as $module => $value) {
      foreach ($value as $type => $settings) {
        drupal_set_message(t('Saved display settings for @module and @type', array('@module' => $module, '@type' => $type)));
        variable_set($module .'_display_settings_'. $type, $settings);
      }
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
            $export[$type] = $settings;
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
          '#value' => "<?php\n/**\n * @file\n * Display suite export data.\n */\nfunction _". $module_name ."_ds_default_settings() {\n  \$data = ". $export_value .";\n  return \$data;\n}",
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
