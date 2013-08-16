<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DsFieldInterface.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Interface for DS plugins.
 */
interface DsFieldInterface {

  /**
   * Renders a field.
   *
   * @param array $field
   *   The field that should be rendered.
   *
   * @return string
   *   Returns the rendered field.
   */
  public function render($field);

  /**
   * Returns the settings for the field settings form.
   *
   * @return array
   *   The rsettings for the form.
   */
  public function settings();

  /**
   * Returns default settings for the settings form.
   *
   * @return array
   *   The default settings.
   */
  public function defaultSettings();

  /**
   * Returns a list of possible formatters for this field.
   *
   * @return array
   *   A list of possible formatters
   */
  public function formatters();

  /**
   * Returns if the field is allowed on the field UI screen.
   *
   * @param $bundle
   *   The bundle we want to display this field on.
   * @param $view_mode
   *   The view mode we want to display this field on.
   */
  public function isAllowed($bundle, $view_mode);

}
