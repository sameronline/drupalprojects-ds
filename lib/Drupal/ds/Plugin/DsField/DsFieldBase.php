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
   * The entity we are working on.
   *
   * @var /Drupal/Core/Entity/EntityInterface
   */
  protected $entity = NULL;

  /**
   * The build of the current entity.
   *
   * @var array
   */
  protected $build = array();

  /**
   * The bundle of the current display.
   *
   * @var string
   */
  protected $bundle = '';

  /**
   * The view mode of the current display.
   *
   * @var string
   */
  protected $view_mode = '';

  /**
   * The name of the field
   *
   * @var string
   */
  protected $field_name = '';

  /**
   * The configuration of the field.
   *
   * @var array
   */
  protected $field_configuration = array();

  /**
   * Constructs a Display Suite field plugin.
   */
  public function __construct($configuration, $plugin_id, $plugin_definition) {
    if (isset($configuration['entity'])) {
      $this->entity = $configuration['entity'];
    }
    if (isset($configuration['bundle'])) {
      $this->bundle = $configuration['bundle'];
    }
    if (isset($configuration['build'])) {
      $this->build = $configuration['build'];
    }
    if (isset($configuration['view_mode'])) {
      $this->view_mode = $configuration['view_mode'];
    }
    if (isset($configuration['field_name'])) {
      $this->field_name = $configuration['field_name'];
    }
    if (isset($configuration['field'])) {
      $this->field_configuration = $configuration['field'];
    }

    parent::__construct($configuration, $plugin_id, $plugin_definition);
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm($settings) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary($settings) {
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
  public function isAllowed() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function getChosenSettings() {
    $field_configuration = $this->getConfiguration();
    $settings = isset($field_configuration['plugin_settings']) ? $field_configuration['plugin_settings'] : array();
    $settings += $this->defaultSettings();

    return $settings;
  }

  /**
   * Gets the current entity.
   */
  public function entity() {
    return $this->entity;
  }

  /**
   * Gets the current entity type.
   */
  public function entityType() {
    return $this->entity->entityType();
  }

  /**
   * Gets the current bundle.
   */
  public function bundle() {
    return $this->bundle;
  }

  /**
   * Gets the view mode
   */
  public function viewMode() {
    return $this->view_mode;
  }

  /**
   * Gets the field configuration
   */
  public function getConfiguration() {
    return $this->field_configuration;
  }

  /**
   * Gets the field name
   */
  public function getName() {
    return $this->field_name;
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
