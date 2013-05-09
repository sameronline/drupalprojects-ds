<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\DynamicCodeField.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\ds\Annotation\DSPlugin;
use Drupal\Core\Annotation\Translation;

/**
 * Defines a generic dynamic code field.
 *
 * @DSPlugin(
 *   id = "dynamic_code_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicCodeField",
 *   module = "ds"
 * )
 */
class DynamicCodeField extends CodePluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\CodePluginBase::code().
   */
  public function code() {
    $definition = $this->getDefinition();
    return $definition['properties']['code']['value'];
  }

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\CodePluginBase::format().
   */
  public function format() {
    $definition = $this->getDefinition();
    return $definition['properties']['code']['format'];
  }
}
