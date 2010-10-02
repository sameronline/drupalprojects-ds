<?php
// $Id$

/**
 * Extend 'Manage display' table with regions and fields.
 *
 * @param $form
 *   The form to add extra regions and fields.
 * @param $form_state
 *   The current form state.
 */
function ds_extend_manage_display_table(&$form, $form_state) {
  // TODO stop if nothing found.

  // Get the entity, bundle and view mode.
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  // Get the fields.
  $fields = ds_get_fields($entity_type, $bundle, $view_mode, TRUE, FALSE);

  $table = &$form['fields'];

  $field_label_options = array(
    'above' => t('Above'),
    'inline' => t('Inline'),
    'hidden' => t('<Hidden>'),
  );

  foreach ($fields as $key => $field) {
    $table[$key] = array(
      '#row_type' => 'field',
      '#attributes' => array('class' => array('draggable', 'tabledrag-leaf')),
      '#region_callback' => 'field_ui_display_overview_row_region', // TODO
      'human_name' => array(
        '#markup' => check_plain($field['title']),
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => 0, // TODO
        '#size' => 3,
        '#attributes' => array('class' => array('field-weight')),
      ),
      'parent_wrapper' => array(
        'parent' => array(
          '#type' => 'select',
          '#required' => FALSE,
          '#options' => array(), // TODO $table['#parent_options'],
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
        '#default_value' => 'default', // TODO
      ),
      'format' => array(
        'type' => array(
          '#type' => 'select',
          '#options' => isset($field['properties']['formatters']) ? $field['properties']['formatters'] : array('default' => t('Default')),
          '#default_value' => 'default',
        ),
      ),
    );
  }

  // Add extra submit handler.
  $form['#submit'][] = 'ds_form_field_ui_display_overview_form_alter_submit';
}

/**
 * Save the region and field settings from the 'Manage display' screen.
 */
function ds_extend_manage_display_table_submit($form, &$form_state) {
  // We'll save later :)
}
