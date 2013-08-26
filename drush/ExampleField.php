<?php
/**
 * @file
 * Contains \Drupal\example_field\Plugin\DsField\ExampleField.
 */

namespace Drupal\example_field\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * @DsField(
 *   id = "example_field_ExampleField",
 *   title = @Translation("ExampleField"),
 *   entity_type = "node",
 * )
 */
class ExampleField extends DsFieldBase {

  public function render($field) {
    return '';
  }

}
