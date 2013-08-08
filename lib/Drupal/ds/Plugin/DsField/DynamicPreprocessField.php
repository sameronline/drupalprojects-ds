<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicPreprocessField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Annotation\DsField;
use Drupal\Core\Annotation\Translation;

/**
 * Defines a generic dynamic preprocess field.
 *
 * @DsField(
 *   id = "dynamic_preprocess_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicPreprocessField",
 *   module = "ds"
 * )
 */
class DynamicPreprocessField extends PreprocessBase {

}
