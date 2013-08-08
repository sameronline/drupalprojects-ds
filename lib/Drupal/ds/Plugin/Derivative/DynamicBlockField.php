<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DynamicBlockField.
 */

namespace Drupal\ds\Plugin\Derivative;

/**
 * Retrieves block plugin definitions for all custom blocks.
 */
class DynamicBlockField extends DynamicField {

  /**
   * {@inheritdoc}
   */
  protected function getType() {
    return DS_FIELD_TYPE_BLOCK;
  }

}
