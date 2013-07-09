  <?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Markup.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Ds field markup base field
 */
abstract class Markup extends PluginBase {

  /**
   * {@inheritdoc}
   */
  public function renderField($field) {
    $key = $this->key();
    if (isset($field['entity']->{$key}->value)) {
      $format = $this->format();
      return check_markup($field['entity']->{$key}->value, $format, '', TRUE);
    }
  }

  /**
   * Gets the key of the field that needs to be rendered.
   */
  protected function key() {
    return '';
  }

  /**
   * Gets the text format.
   */
  protected function format() {
    return 'filtered_html';
  }

}
