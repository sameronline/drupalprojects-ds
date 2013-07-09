<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\PluginBase.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;

/**
 * Base class for all the ds plugins
 */
abstract class PluginBase extends ComponentPluginBase implements PluginBaseInterface {

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBaseInterface::renderField()
   */
  public function renderField($field) {
    return '';
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBaseInterface::settings()
   */
  public function settings() {
    return array();
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBaseInterface::defaultSettings()
   */
  public function defaultSettings() {
    return array();
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBaseInterface::formatters()
   */
  public function formatters() {
    return array();
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBaseInterface::displays()
   */
  public function displays() {
    return array();
  }

}
