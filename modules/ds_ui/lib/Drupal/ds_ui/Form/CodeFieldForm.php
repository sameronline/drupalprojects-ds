<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\CodeFieldForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\ds_ui\Form\FieldFormBase;

/**
 * Configures code fields.
 */
class CodeFieldForm extends FieldFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'ds_field_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state, $field_key = '') {

    $form = parent::buildForm($form, $form_state, $field_key);

    $field = $this->field;

    if (empty($field_key)) {
      $form['#title'] = 'Add a code field';
    }
    else {
      $form['#title'] = 'Edit a code field';
    }

    $form['code'] = array(
      '#type' => 'text_format',
      '#title' => t('Field code'),
      '#default_value' => isset($field['properties']['code']['value']) ? $field['properties']['code']['value'] : '',
      '#format' => isset($field['properties']['code']['format']) ? $field['properties']['code']['format'] : 'ds_code',
      '#base_type' => 'textarea',
      '#required' => TRUE,
    );

    $form['use_token'] = array(
      '#type' => 'checkbox',
      '#title' => t('Token'),
      '#description' => t('Toggle this checkbox if you are using tokens in this field.'),
      '#default_value' => isset($field['properties']['use_token']) ? $field['properties']['use_token'] : '',
    );

    // Token support.
    if (module_exists('token')) {

      $form['tokens'] = array(
        '#title' => t('Tokens'),
        '#type' => 'container',
        '#states' => array(
          'invisible' => array(
            'input[name="use_token"]' => array('checked' => FALSE),
          ),
        ),
      );
      $form['tokens']['help'] = array(
        '#theme' => 'token_tree',
        '#token_types' => 'all',
        '#global_types' => FALSE,
        '#dialog' => TRUE,
      );
    }
    else {
      $form['use_token']['#description'] = t('Toggle this checkbox if you are using tokens in this field. If the token module is installed, you get a nice list of all tokens available in your site.');
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {
    parent::validateForm($form, $form_state);

    $field = &$this->field;
    $field['field_type'] = DS_FIELD_TYPE_CODE;
    $field['properties']['code'] = $form_state['values']['code'];
    $field['properties']['use_token'] = $form_state['values']['use_token'];
  }

}
