<?php

/**
 * @file
 * Contains \Drupal\ds_test\Plugin\DsField\TestField.
 */

namespace Drupal\ds_test\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Test code field from plugin.
 *
 * @DsField(
 *   id = "test_field",
 *   title = @Translation("Test code field from plugin"),
 *   entity_type = "node"
 * )
 */
class TestField extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return 'Test code field on node ' . $field['entity']->id();
  }

}
