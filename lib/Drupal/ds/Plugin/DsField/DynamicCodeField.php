<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicCodeField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Defines a generic dynamic code field.
 *
 * @DsField(
 *   id = "dynamic_code_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicCodeField"
 * )
 */
class DynamicCodeField extends CodeBase {

  /**
   * {@inheritdoc}
   */
  public function code() {
    $definition = $this->getPluginDefinition();
    return $definition['properties']['code']['value'];
  }

  /**
   * {@inheritdoc}
   */
  public function format() {
    $definition = $this->getPluginDefinition();
    return $definition['properties']['code']['format'];
  }

  /**
   * {@inheritdoc}
   */
  public function isAllowed() {
    $definition = $this->getPluginDefinition();
    return DsFieldBase::dynamicFieldIsAllowed($definition, $this->bundle(), $this->viewMode());
  }
}
