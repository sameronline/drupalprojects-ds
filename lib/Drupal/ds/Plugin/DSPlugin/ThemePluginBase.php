<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\ThemePluginBase.
 */

namespace Drupal\ds\Plugin\DSPlugin;

/**
 * The base plugin to create DS theme fields.
 */
abstract class ThemePluginBase extends PluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::renderField().
   */
  public function renderField($field) {
    $format = $this->formatter();
    return theme($format, $field);
  }

  /**
   * Returns the formatter for the theming function.
   */
  protected function formatter() {
    return '';
  }

}
