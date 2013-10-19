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
 *   entity_type = "node"
 * )
 */
class ExampleField extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function formatters() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    return TRUE;
  }

}
