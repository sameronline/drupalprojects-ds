<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\PreprocessFieldForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\ds_ui\Form\FieldFormBase;

/**
 * Configures classes used by Display Suite.
 */
class PreprocessFieldForm extends FieldFormBase {

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

    $form['info'] = array(
      '#markup' => t('The machine name of this field must reflect the key in the variables, e.g. "submitted". So in most cases, it is very likely you will have to manually edit the machine name as well, which can not be changed anymore after saving. Note that this field type works best on Nodes.'),
      '#weight' => -10,
    );
    return parent::buildForm($form, $form_state, $field_key);
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {
    parent::validateForm($form, $form_state);

    $field = &$this->field;
    $field['field_type'] = DS_FIELD_TYPE_PREPROCESS;
  }

}
