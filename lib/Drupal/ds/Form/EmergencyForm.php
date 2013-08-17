<?php

/**
 * @file
 * Contains \Drupal\ds\Form\EmergencyForm.
 */

namespace Drupal\ds\Form;

use Drupal\Core\Form\FormInterface;

/**
 * Emergency form for DS.
 */
class EmergencyForm implements FormInterface {

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'ds_emergy_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {
    $form['ds_fields_error'] = array(
      '#type' => 'fieldset',
      '#title' => t('Fields error'),
    );

    $form['ds_fields_error']['disable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable attaching fields via Display Suite'),
      '#description' => t('In case you get an error after configuring a layout printing a message like "Fatal error: Unsupported operand types", you can temporarily disable adding fields from DS by toggling this checkbox. You probably are trying to render an node inside a node, for instance through a view, which is simply not possible. See <a href="http://drupal.org/node/1264386">http://drupal.org/node/1264386</a>.'),
      '#default_value' => \Drupal::config('ds.settings')->get('disable', FALSE),
      '#weight' => 0,
    );

    $form['ds_fields_error']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Disable/enable field attach'),
      '#submit' => array(array($this, 'submitFieldAttach')),
      '#weight' => 1,
    );

    if (module_exists('ds_extras')) {
      $region_blocks = \Drupal::config('ds.extras')->get('region_blocks', array());
      if (!empty($region_blocks)) {

        $region_blocks_options = array();
        foreach ($region_blocks as $key => $info) {
          $region_blocks_options[$key] = $info['title'];
        }

        $form['region_to_block'] = array(
          '#type' => 'fieldset',
          '#title' => t('Block regions'),
        );

        $form['region_to_block']['remove_block_region'] = array(
          '#type' => 'checkboxes',
          '#options' => $region_blocks_options,
          '#description' => t('In case you renamed a content type, you will not see the configured block regions anymore, however the block on the block settings page is still available. On this screen you can remove orphaned block regions.'),
        );

        $form['region_to_block']['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Remove block regions'),
          '#submit' => array(array($this, 'submitRegionToBlock')),
          '#weight' => 1,
        );
      }
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {
    // empty
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    // empty
  }

  /**
   * Submit callback for the fields error form.
   */
  public function submitFieldAttach(array &$form, array &$form_state) {
    \Drupal::config('ds.settings')->set('disable', $form_state['values']['disable'])->save();
    drupal_set_message(t('The configuration options have been saved.'));
  }

  /**
   * Submit callback for the region to block form
   */
  public function submitRegionToBlock(array &$form, array &$form_state) {
    if (isset($form_state['values']['remove_block_region'])) {
      $save = FALSE;
      $region_blocks = \Drupal::config('ds.extras')->get('region_blocks', array());
      $remove = $form_state['values']['remove_block_region'];
      foreach ($remove as $key => $value) {
        if ($key === $value) {
          $save = TRUE;
          db_delete('block')
            ->condition('delta', $key)
            ->condition('module', 'ds_extras')
            ->execute();
          unset($region_blocks[$key]);
        }
      }

      if ($save) {
        drupal_set_message(t('Block regions were removed.'));
        \Drupal::config('ds.extras')->et('region_blocks', $region_blocks);
      }
    }
    else {
      drupal_set_message(t('No block regions were removed.'));
    }
  }

}
