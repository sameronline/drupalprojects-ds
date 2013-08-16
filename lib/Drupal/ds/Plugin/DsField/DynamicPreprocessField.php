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

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    $definition = $this->getPluginDefinition();

    if (!isset($definition['ui_limit'])) {
      return TRUE;
    }

    $limits = $definition['ui_limit'];
    foreach ($limits as $limit) {
      list($bundle_limit, $view_mode_limit) = explode('|', $limit);

      if (($bundle_limit == $bundle || $bundle_limit == '*') && $view_mode_limit == $view_mode) {
        return FALSE;
      }
    }

    return TRUE;
  }

}
