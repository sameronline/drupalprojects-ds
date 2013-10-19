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
   */
  public function build();

  /**
   * Returns the settings form for the field.
   *
   * @param $field
   *   Contains all the general configuration of the field.
   *   Contains the settings of the field.
   *
   * @return array
   *   A render array containing the form.
   */
  public function settingsForm($settings);

  /**
   * Returns the summary of the chosen settings.
   *
   * @param $field
   *   Contains all the general configuration of the field.
   * @param $settings
   *   Contains the settings of the field.
   *
   * @return array
   *   A render array containing the summary.
   */
  public function settingsSummary($settings);

  /**
   * Returns the default settings.
   *
   * @return array
   *   An array containing the default settings in a key value way.
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
   */
  public function isAllowed();

}
