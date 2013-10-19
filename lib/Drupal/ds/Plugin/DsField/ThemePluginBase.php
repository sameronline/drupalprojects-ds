<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\ThemePluginBase.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * The base plugin to create DS theme fields.
 */
abstract class ThemePluginBase extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $format = $this->formatter();

    return array(
      '#markup' => theme($format, $field),
    );
  }

  /**
   * Returns the formatter for the theming function.
   */
  protected function formatter() {
    return '';
  }

}
