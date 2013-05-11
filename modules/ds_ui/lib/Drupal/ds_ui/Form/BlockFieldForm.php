<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\BlockFieldForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\ds_ui\Form\FieldFormBase;

/**
 * Configures classes used by Display Suite.
 */
class BlockFieldForm extends FieldFormBase {

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

    $blocks = array();
    foreach ($this->modeleHandler->getImplementations('block_info') as $module) {
      $module_blocks = $this->modeleHandler->invoke($module, 'block_info');
      if ($module_blocks) {
        foreach ($module_blocks as $module_key => $info) {
          $blocks[drupal_ucfirst($module)][$module . '|' . $module_key] = $info['info'];
        }
      }
    }
    ksort($blocks);

    $form['block_identity']['block'] = array(
      '#type' => 'select',
      '#options' => $blocks,
      '#title' => t('Block'),
      '#required' => TRUE,
      '#default_value' => isset($field['properties']['block']) ? $field['properties']['block'] : '',
    );
    $form['block_identity']['block_render'] = array(
      '#type' => 'select',
      '#options' => array(
        DS_BLOCK_TEMPLATE => t('Default'),
        DS_BLOCK_TITLE_CONTENT => t('Show block title + content'),
        DS_BLOCK_CONTENT => t('Show only block content'),
      ),
      '#title' => t('Layout'),
      '#required' => TRUE,
      '#default_value' => isset($field['properties']['block_render']) ? $field['properties']['block_render'] : '',
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
    $field['properties']['block_render'] = $form_state['values']['block_render'];
  }

}
