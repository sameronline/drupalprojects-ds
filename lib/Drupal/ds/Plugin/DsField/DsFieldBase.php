<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DsFieldBase.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;

/**
 * Base class for all the ds plugins
 */
abstract class DsFieldBase extends ComponentPluginBase implements DsFieldInterface {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return '';
  }

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function formatters() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function getChosenSettings($field) {
    $settings = isset($field['plugin_settings']) ? $field['plugin_settings'] : array();
    $settings += $this->defaultSettings();

    return $settings;
  }

}
