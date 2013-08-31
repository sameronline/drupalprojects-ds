<?php

/**
 * @file
 * Contains \Drupal\ds_test\Plugin\DsField\TestFieldZeroFloat.
 */

namespace Drupal\ds_test\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Test code field that returns zero as a float.
 *
 * @DsField(
 *   id = "test_field_zero_float",
 *   title = @Translation("Test code field that returns zero as a float"),
 *   entity_type = "node"
 * )
 */
class TestFieldZeroFloat extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return 0.0;
  }

}
