<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\BlockFieldForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\ds_ui\Form\FieldFormBase;
use Drupal\Core\Controller\ControllerInterface;

/**
 * Configure block fields.
 */
class BlockFieldForm extends FieldFormBase implements ControllerInterface {

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

    $manager = \Drupal::service('plugin.manager.block');

    $blocks = array();
    foreach ($manager->getDefinitions() as $plugin_id => $plugin_definition) {
      $blocks[$plugin_id] = $plugin_definition['admin_label'];
    }
    asort($blocks);

    $form['block_identity']['block'] = array(
      '#type' => 'select',
      '#options' => $blocks,
      '#title' => t('Block'),
      '#required' => TRUE,
      '#default_value' => isset($field['properties']['block']) ? $field['properties']['block'] : '',
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {
    parent::validateForm($form, $form_state);

    $field = &$this->field;
    $field['field_type'] = DS_FIELD_TYPE_BLOCK;
    $field['properties'] = array();
    $field['properties']['block'] = $form_state['values']['block'];
  }

}
