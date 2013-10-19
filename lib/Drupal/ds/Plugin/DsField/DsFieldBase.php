<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DsFieldBase.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Component\Plugin\PluginBase as ComponentPluginBase;

/**
 * Base class for all the ds plugins.
 */
abstract class DsFieldBase extends ComponentPluginBase implements DsFieldInterface {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm($field, $settings) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary($field, $settings) {
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

  /**
   * Checks if the dynamic field is allowed to display on this field UI page.
   *
   * This is a helper function for the dynamic plugins defined in the UI.
   *
   * @param array $defintion
   *   The defintion of the plugin
   * @param string $bundle
   *   The bundle you're performing the check for.
   * @param string $view_mode
   *   The view mode you're performing the check for.
   */
  public static function dynamicFieldIsAllowed(array $definition, $bundle, $view_mode) {

    if (!isset($definition['ui_limit'])) {
      return TRUE;
    }

    $limits = $definition['ui_limit'];
    foreach ($limits as $limit) {
      list($bundle_limit, $view_mode_limit) = explode('|', $limit);

      if (($bundle_limit == $bundle || $bundle_limit == '*') && ($view_mode_limit == $view_mode || $view_mode_limit == '*')) {
        return TRUE;
      }
    }

    // When the current bundle view_mode combination is not allowed we shouldn't
    // show the field.
    return FALSE;
  }

}
