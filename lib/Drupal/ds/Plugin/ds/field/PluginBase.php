<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\PluginBase.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;

/**
 * Base class for all the ds plugins
 */
abstract class PluginBase extends ComponentPluginBase {

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

  /**
   * Returns a list of displays where the field will be showed.
   *
   * Only used for the manage display screen so you can limit fields to show
   * based on bundles or view modes the values are always in the form of
   * $bundle|$view_mode
   *
   * You may use * to select all.
   * Make sure you use the machine name.
   *
   * @return array
   *   Leave empty if you don't want to limit displaying the field.
   *   Returns FALSE if this field shouldn't be shown anywhere.
   */
  public function displays() {
    $displays = array(
      'article|full',
      '*|search_index'
    );

    return $displays;
  }

}
