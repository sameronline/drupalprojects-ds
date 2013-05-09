  <?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\Markup.
 */

namespace Drupal\ds\Plugin\DSPlugin;

/**
 * The base plugin to create DS field fields.
 */
abstract class Markup extends PluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::renderField().
   */
  public function renderField($field) {
    $key = $this->key();
    if (isset($field['entity']->{$key})) {
      $format = $this->format();
      return check_markup($field['entity']->{$key}, $format, '', TRUE);
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
