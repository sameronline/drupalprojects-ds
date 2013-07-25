<?php

/**
 * Field template settings form
 */
function ds_field_template_settings_form(array &$form, array &$form_state, array $context) {
  $functions = module_invoke_all('ds_field_theme_functions_info');
  $default_field_function = config('ds.settings')->get('ft-default');
  if (empty($default_field_function)) {
    $default_field_function = 'theme_field';
  }
  $key = $context['instance']['field_name'];

  $field_settings = isset($form_state['plugin_settings'][$key]) ? $form_state['plugin_settings'][$key]['ft'] : array();

  $field_function = isset($field_settings['func']) ? $field_settings['func'] : $default_field_function;
  $field_classes = _ds_classes('fields');

  $form['ft'] = array(
    '#weight' => 20,
  );

  // Functions.
  $form['ft']['func'] = array(
    '#title' => t('Choose a Field Template'),
    '#type' => 'select',
    '#options' => $functions,
    '#default_value' => $field_function,
    '#attributes' => array(
      'class' => array('ds-extras-field-template'),
    ),
  );

  // Field classes.
  if (!empty($field_classes)) {
    $field_classes_select = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $field_classes,
      '#title' => t('Choose additional CSS classes for the field'),
      '#default_value' => isset($field_settings['classes']) ? explode(' ', $field_settings['classes']) : array(),
      '#prefix' => '<div class="field-classes">',
      '#suffix' => '</div>',
    );
    $form['ft']['classes'] = $field_classes_select;
  }
  else {
    $form['ft']['classes'] = array(
      '#type' => 'value',
      '#value' => array(''),
    );
  }

  // Wrappers and label.
  $wrappers = array(
    'lb' => array('title' => t('Label')),
    'ow' => array('title' => t('Outer wrapper')),
    'fis' => array('title' => t('Field items')),
    'fi' => array('title' => t('Field item')),
  );

  foreach ($wrappers as $wrapper_key => $value) {

    $classes = array(
      'field-name-' . strtr($key, '_', '-'),
    );
    $form['ft'][$wrapper_key] = array(
      '#type' => 'checkbox',
      '#title' => $value['title'],
      '#prefix' => '<div class="ft-group ' . $wrapper_key . '">',
      '#default_value' => isset($field_settings[$wrapper_key]) ? $field_settings[$wrapper_key] : FALSE,
    );
    $form['ft'][$wrapper_key . '-el'] = array(
      '#type' => 'textfield',
      '#title' => t('Element'),
      '#size' => '10',
      '#description' => t('E.g. div, span, h2 etc.'),
      '#default_value' => isset($field_settings[$wrapper_key . '-el']) ? $field_settings[$wrapper_key . '-el'] : '',
    );
    $form['ft'][$wrapper_key . '-cl'] = array(
      '#type' => 'textfield',
      '#title' => t('Classes'),
      '#size' => '10',
      '#default_value' => isset($field_settings[$wrapper_key . '-cl']) ? $field_settings[$wrapper_key . '-cl'] : '',
      '#description' => t('E.g.') .' ' . implode(', ', $classes),
    );
    $form['ft'][$wrapper_key . '-at'] = array(
      '#type' => 'textfield',
      '#title' => t('Attributes'),
      '#size' => '20',
      '#default_value' => isset($field_settings[$wrapper_key . '-at']) ? $field_settings[$wrapper_key . '-at'] : '',
      '#description' => t('E.g. name="anchor"'),
    );

    // Hide colon.
    if ($wrapper_key == 'lb') {
      $form['ft']['lb-col'] = array(
        '#type' => 'checkbox',
        '#title' => t('Hide label colon'),
        '#default_value' => isset($field_settings['lb-col']) ? $field_settings['lb-col'] : FALSE,
        '#attributes' => array(
          'class' => array('colon-checkbox'),
        ),
      );
    }
    if ($wrapper_key == 'fi') {
      $form['ft']['fi-odd-even'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add odd/even classes'),
        '#default_value' => isset($field_settings['fi-odd-even']) ? $field_settings['fi-odd-even'] : FALSE,
      );
    }
    $form['ft'][$wrapper_key . '-def-at'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add default attributes'),
      '#default_value' => isset($field_settings[$wrapper_key . '-def-at']) ? $field_settings[$wrapper_key . '-def-at'] : FALSE,
      '#suffix' => ($wrapper_key == 'ow') ? '' : '</div><div class="clearfix"></div>',
    );

    // Default classes for outer wrapper.
    if ($wrapper_key == 'ow') {
      $form['ft'][$wrapper_key . '-def-cl'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add default classes'),
        '#default_value' => isset($field_settings[$wrapper_key . '-def-cl']) ? $field_settings[$wrapper_key . '-def-cl'] : FALSE,
        '#suffix' => '</div><div class="clearfix"></div>',
      );
    }
  }

  // Another label needs some other stuff.
  unset($form['ft']['lb']['#description']);
  $form['ft']['lb']['#type'] = 'textfield';
  $form['ft']['lb']['#size'] = '10';
  $form['ft']['lb']['#attributes'] = array('class' => array('label-change'));
  $form['ft']['lb']['#default_value'] = isset($field_settings['lb']) ? $field_settings['lb'] : '';
}
