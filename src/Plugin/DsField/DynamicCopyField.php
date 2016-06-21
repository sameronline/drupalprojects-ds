<?php

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Form\FormStateInterface;
use Drupal\ds\Plugin\DsPluginManager;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Defines a generic dynamic field that holds a copy of an exisitng ds field.
 *
 * @DsField(
 *   id = "dynamic_copy_field",
 *   deriver = "Drupal\ds\Plugin\Derivative\DynamicCopyField",
 * )
 */
class DynamicCopyField extends DsFieldBase {

  /**
   * The loaded instance.
   *
   * @var \Drupal\ds\Plugin\DsField\DsFieldInterface;
   */
  private $field_instance;

  /**
   * Constructs a Display Suite field plugin.
   */
  public function __construct($configuration, $plugin_id, $plugin_definition, DsPluginManager $plugin_Manager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->field_instance = $plugin_Manager->createInstance($plugin_definition['properties']['ds_plugin'], $configuration);
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('plugin.manager.ds')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    return $this->field_instance->build();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm($form, FormStateInterface $form_state) {
    return $this->field_instance->settingsForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary($settings) {
    return $this->field_instance->settingsSummary($settings);
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration() {
    return $this->field_instance->getConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function setConfiguration(array $configuration) {
    return $this->field_instance->setConfiguration($configuration);
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function formatters() {
    return $this->field_instance->formatters();
  }

}
