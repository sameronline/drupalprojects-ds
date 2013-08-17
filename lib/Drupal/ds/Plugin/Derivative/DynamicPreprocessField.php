<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DynamicPreprocessField.
 */

namespace Drupal\ds\Plugin\Derivative;

/**
 * Retrieves dynamic preprocess field plugin definitions.
 */
class DynamicPreprocessField extends DynamicField {

  /**
   * {@inheritdoc}
   */
  protected function getType() {
    return DS_FIELD_TYPE_PREPROCESS;
  }

}
