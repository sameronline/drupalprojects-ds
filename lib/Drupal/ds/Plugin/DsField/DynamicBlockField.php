<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicBlockField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Annotation\DsField;
use Drupal\Core\Annotation\Translation;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Defines a generic dynamic block field.
 *
 * @DsField(
 *   id = "dynamic_block_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicBlockField",
 *   provider = "block"
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

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    $definition = $this->getPluginDefinition();

    return DsFieldBase::dynamicFieldIsAllowed($definition, $bundle, $view_mode);
  }

}
