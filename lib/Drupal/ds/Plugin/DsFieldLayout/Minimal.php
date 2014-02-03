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
 *   theme = "theme_ds_field_minimal"
 * )
 */
class Minimal extends DsFieldLayoutBase {

  /**
   * {@inheritdoc}
   */
  public function alterForm(&$form) {
    parent::alterForm($form);

    $config = $this->getConfiguration();
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

    $config['lb'] = '';
    $config['lb-col'] = \Drupal::config('ds.settings')->get('ft-kill-colon');

    return $config;
  }

}
