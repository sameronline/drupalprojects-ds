<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsFieldLayout\Minimal.
 */

namespace Drupal\ds\Plugin\DsFieldLayout;

use Drupal\Component\Utility\String;

/**
 * Plugin that renders the title of a node.
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
  public function alterForm(&$form, &$form_state) {

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

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array(
      'lb' => '',
      'lb-col' => \Drupal::config('ds.settings')->get('ft-kill-colon'),
    );
  }

}
