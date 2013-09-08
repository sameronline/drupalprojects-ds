<?php

/**
 * @file
 * Contains \Drupal\ds_test\Plugin\DsField\TestFieldEmptyString.
 */

namespace Drupal\ds_test\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Test code field that returns an empty string.
 *
 * @DsField(
 *   id = "test_field_empty_string",
 *   title = @Translation("Test code field that returns an empty string"),
 *   entity_type = "node"
 * )
 */
class TestFieldEmptyString extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return '';
  }

}
