<?php

/**
 * @file
 * Contains \Drupal\ds_test\Plugin\DsField\TestFieldFalse.
 */

namespace Drupal\ds_test\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Test code field that returns FALSE.
 *
 * @DsField(
 *   id = "test_field_false",
 *   title = @Translation("Test code field that returns FALSE"),
 *   entity_type = "node"
 * )
 */
class TestFieldFalse extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return FALSE;
  }

}
