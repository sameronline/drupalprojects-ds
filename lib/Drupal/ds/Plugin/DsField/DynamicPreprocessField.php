<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicPreprocessField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Defines a generic dynamic preprocess field.
 *
 * @DsField(
 *   id = "dynamic_preprocess_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicPreprocessField"
 * )
 */
class DynamicPreprocessField extends PreprocessBase {

  /**
   * {@inheritdoc}
   */
  public function isAllowed() {
    $definition = $this->getPluginDefinition();

    return DsFieldBase::dynamicFieldIsAllowed($definition, $this->bundle(), $this->viewMode());
  }

}
