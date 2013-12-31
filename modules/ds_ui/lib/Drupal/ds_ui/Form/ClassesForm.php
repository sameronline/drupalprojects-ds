<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\ClassesForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\Core\Form\ConfigFormBase;

/**
 * Configures classes used by wrappers and regions.
 */
class ClassesForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'ds_classes_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {
    $config = $this->configFactory->get('ds.classes');

    $form['regions'] = array(
      '#type' => 'textarea',
      '#title' => t('CSS classes for regions'),
      '#default_value' => implode("\n", $config->get('regions')),
      '#description' => t('Configure CSS classes which you can add to regions on the "manage display" screens. Add multiple CSS classes line by line.<br />If you want to have a friendly name, separate class and friendly name by |, but this is not required. eg:<br /><em>class_name_1<br />class_name_2|Friendly name<br />class_name_3</em>')
    );

    $form['fields'] = array(
      '#type' => 'textarea',
      '#title' => t('CSS classes for fields'),
      '#default_value' =>  implode("\n", $config->get('fields')),
      '#description' => t('Configure CSS classes which you can add to fields on the "manage display" screens. Add multiple CSS classes line by line.<br />If you want to have a friendly name, separate class and friendly name by |, but this is not required. eg:<br /><em>class_name_1<br />class_name_2|Friendly name<br />class_name_3</em>')
    );

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    parent::submitForm($form, $form_state);
    $config = $this->configFactory->get('ds.classes');
    $config->set('regions', explode("\n", str_replace("\r", '', $form_state['values']['regions'])))
      ->set('fields', explode("\n", str_replace("\r", '', $form_state['values']['fields'])))
      ->save();
  }

}
