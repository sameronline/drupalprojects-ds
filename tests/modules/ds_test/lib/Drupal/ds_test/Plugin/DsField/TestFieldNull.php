<?php

/**
 * @file
 * Contains \Drupal\ds_test\Plugin\DsField\TestFieldNull.
 */

namespace Drupal\ds_test\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Test code field that returns NULL.
 *
 * @DsField(
 *   id = "test_field_null",
 *   title = @Translation("Test code field that returns NULL"),
 *   entity_type = "node"
 * )
 */
class TestFieldNull extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return NULL;
  }

}
