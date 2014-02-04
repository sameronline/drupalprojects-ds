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
    // Do nothing
  }

  /**
   * {@inheritdoc}
   */
  public function massageOut(&$field_settings, $values) {
    // Save nothing
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
    return array();
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
