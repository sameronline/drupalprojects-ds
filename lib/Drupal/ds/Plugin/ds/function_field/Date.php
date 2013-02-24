<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\function_field\PostDate.
 */

namespace Drupal\ds\Plugin\ds\function_field;

/**
 * The base plugin to create DS post date function fields.
 */
abstract class PostDate extends FunctionFieldPluginBase {

  /**
   * Implements \Drupal\ds\Plugin\ds\FieldPluginBase\renderField().
   */
  public function renderField($field) {
    $date_format = str_replace('ds_post_date_', '', $field['formatter']);
    return format_date($field['entity']->created, $date_format);
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\FieldPluginBase::formatters().
   */
  public function formatters() {
    $date_types = system_get_date_formats();
    $date_formatters = array();
    foreach ($date_types as $machine_name => $value) {
      if ($value['locked']) {
        continue;
      }
      $date_formatters['ds_post_date_' . $machine_name] = t($value['name']);
    }

    return $date_formatters;
  }

}
