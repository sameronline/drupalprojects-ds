<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicCodeField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Annotation\DsField;
use Drupal\Core\Annotation\Translation;

/**
 * Defines a generic dynamic code field.
 *
 * @DsField(
 *   id = "dynamic_code_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicCodeField",
 *   module = "ds"
 * )
 */
class DynamicCodeField extends CodePluginBase {

  /**
   * {@inheritdoc}
   */
  public function code() {
    $definition = $this->getDefinition();
    return $definition['properties']['code']['value'];
  }

  /**
   * {@inheritdoc}
   */
  public function format() {
    $definition = $this->getDefinition();
    return $definition['properties']['code']['format'];
  }
}
