<?php

/**
 * @file
 * Administrative functions for managing CSS classes.
 */

/**
 * Menu callback, show CSS classes form.
 */
function ds_classes_form($form, $form_state) {
  $form = array();

  $form['regions'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS classes for regions'),
    '#default_value' => config('ds.classes')->get('region'),
    '#description' => t('Configure CSS classes which you can add to regions on the "manage display" screens. Add multiple CSS classes line by line.<br />If you want to have a friendly name, separate class and friendly name by |, but this is not required. eg:<br /><em>class_name_1<br />class_name_2|Friendly name<br />class_name_3</em>')
  );

  $form['fields'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS classes for fields'),
    '#default_value' => config('ds.classes')->get('fields'),
    '#description' => t('Configure CSS classes which you can add to fields on the "manage display" screens. Add multiple CSS classes line by line.<br />If you want to have a friendly name, separate class and friendly name by |, but this is not required. eg:<br /><em>class_name_1<br />class_name_2|Friendly name<br />class_name_3</em>')
  );

  return system_config_form($form, $form_state);
}

/**
 * Submit callback: save the classes.
 */
function ds_classes_form_submit($form, $form_state) {
  config('ds.classes')
    ->set('regions', $form_state['values']['regions'])
    ->set('fields', $form_state['values']['fields'])
    ->save();
}
