<?php

/**
 * @file
 * Definition of Drupal\ds\DS.
 */

namespace Drupal\ds;

/**
 * Display Suite master object
 */
class DS {

  /**
   * Returns the valid types of plugins that can be used.
   *
   * @return array
   *   An array of plugin type strings.
   */
  public static function getFieldPluginTypes() {
    return array(
      'function_field',
      'code_field',
      'block_field',
      'preprocess_field',
      'ignore_field',
      'theme_field'
    );
  }

}
