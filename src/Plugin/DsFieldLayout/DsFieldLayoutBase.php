<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsFieldLayout\DsFieldLayoutBase.
 */

namespace Drupal\ds\Plugin\DsFieldLayout;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;
use Drupal\Component\Utility\String;
use Drupal\ds\Ds;

/**
 * Base class for all the ds plugins.
 */
abstract class DsFieldLayoutBase extends ComponentPluginBase implements DsFieldLayoutInterface {

  /**
   * Constructs a Display Suite field plugin.
   */
  public function __construct($configuration, $plugin_id, $plugin_definition) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->configuration += $this->defaultConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function alterForm(&$form) {
    $config = $this->getConfiguration();

    $form['lb'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#size' => '10',
      '#default_value' => String::checkPlain($config['lb']),
    );
    $form['lb-col'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show label colon'),
      '#default_value' => $config['lb-col'],
      '#attributes' => array(
        'class' => array('colon-checkbox'),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function massageRenderValues(&$field_settings, $values) {
    if (!empty($values['lb'])) {
      $field_settings['lb'] = $values['lb'];
    }
    if (!(empty($values['lb-col']))) {
      $field_settings['lb-col'] = TRUE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getThemeFunction() {
    return $this->pluginDefinition['theme'];
  }

  /**
   *
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $config = array();
    $config['lb'] = '';
    $config['lb-col'] = \Drupal::config('ds.settings')->get('ft-kill-colon');

    return $config;
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(array $configuration) {
    $this->configuration = $configuration + $this->configuration;
  }

}
