<?php

/**
 * @file
 * Definition of Drupal\ds\Plugin\ds\FieldPluginBase.
 */

namespace Drupal\ds\Plugin\ds;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;

/**
 * Base class for all the ds plugins
 */
abstract class FieldPluginBase extends ComponentPluginBase {

  /**
   * Renders a field.
   *
   * @param array $field
   *   The field that should be rendered.
   *
   * @return string
   *   Returns the rendered field.
   */
  abstract public function renderField($field);

  /**
   * Returns a settings form for the field.
   *
   * @return array
   *   The render array building the form.
   */
  public function settingsForm() {
    return array();
  }

  /**
   * Returns default settings for the settings form.
   *
   * @return array
   *   The default settings.
   */
  public function defaultSettings() {
    return array();
  }

  /**
   * Returns a list of possible formatters for this field.
   *
   * @return array
   *   A list of possible formatters
   */
  public function formatters() {
    return array();
  }

}
