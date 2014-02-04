<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsFieldLayout\Minimal.
 */

namespace Drupal\ds\Plugin\DsFieldLayout;

use Drupal\Component\Utility\String;

/**
 * Plugin for the minimal field template.
 *
 * @DsFieldLayout(
 *   id = "minimal",
 *   title = @Translation("Minimal"),
 *   theme = "theme_ds_field_minimal",
 *   path = "includes/theme.inc"
 * )
 */
class Minimal extends DsFieldLayoutBase {

  /**
   * {@inheritdoc}
   */
  public function alterForm(&$form) {
    // Field classes.
    $config = $this->getConfiguration();
    $field_classes = Ds::getClasses('field');
    if (!empty($field_classes)) {
      $form['classes'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $field_classes,
        '#title' => t('Choose additional CSS classes for the field'),
        '#default_value' => $config['classes'],
        '#prefix' => '<div class="field-classes">',
        '#suffix' => '</div>',
      );
    }
    else {
      $form['classes'] = array(
        '#type' => 'value',
        '#value' => array(''),
      );
    }
    $form['lb'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#size' => '10',
      '#default_value' => String::checkPlain($config['lb']),
    );
    $form['lb-col'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide label colon'),
      '#default_value' => $config['lb-col'],
      '#attributes' => array(
        'class' => array('colon-checkbox'),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $config = parent::defaultConfiguration();

    $config['classes'] = array();
    $config['lb'] = '';
    $config['lb-col'] = \Drupal::config('ds.settings')->get('ft-kill-colon');

    return $config;
  }

  /**
   * {@inheritdoc}
   */
  public function massageFormValues(&$field_settings, $values) {
    if (isset($values['classes'])) {
      $classes = is_array($values['classes']) ? implode(' ', $values['classes']) : $values['classes'];
      if (!empty($classes)) {
        $field_settings['classes'] = $classes;
      }
    }
    if (!empty($values['lb'])) {
      $field_settings['lb'] = $values['lb'];
    }
    if (!(empty($values['lb-col']))) {
      $field_settings['lb-col'] = TRUE;
    }
  }

}
