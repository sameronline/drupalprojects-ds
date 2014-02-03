<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsFieldLayout\DsFieldLayoutBase.
 */

namespace Drupal\ds\Plugin\DsFieldLayout;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;
use Drupal\ds\Ds;

/**
 * Base class for all the ds plugins.
 */
abstract class DsFieldLayoutBase extends ComponentPluginBase {

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
  }

  /**
   *
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array(
      'classes' => array(),
    );
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
