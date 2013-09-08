<?php

/**
 * @file
 * Contains \Drupal\ds_test\Plugin\DsField\TestFieldZeroInt.
 */

namespace Drupal\ds_test\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Test code field that returns zero as an integer.
 *
 * @DsField(
 *   id = "test_field_zero_int",
 *   title = @Translation("Test code field that returns zero as an integer"),
 *   entity_type = "node"
 * )
 */
class TestFieldZeroInt extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return 0;
  }

}
