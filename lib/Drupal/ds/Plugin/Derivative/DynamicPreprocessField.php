<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DynamicPreprocessField.
 */

namespace Drupal\ds\Plugin\Derivative;

/**
 * Retrieves block plugin definitions for all custom blocks.
 */
class DynamicPreprocessField extends DynamicField {

  /**
   * {@inheritdoc}
   */
  protected function getType() {
    return DS_FIELD_TYPE_PREPROCESS;
  }

}
