<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicBlockField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Annotation\DsField;
use Drupal\Core\Annotation\Translation;

/**
 * Defines a generic dynamic block field.
 *
 * @DsField(
 *   id = "dynamic_block_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicBlockField",
 *   module = "ds"
 * )
 */
class DynamicBlockField extends BlockBase {

  /**
   * {@inheritdoc}
   */
  protected function blockPluginId() {
    $definition = $this->getPluginDefinition();
    return $definition['properties']['block'];
  }
}
