<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Date.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * The base plugin to create DS post date function fields.
 */
abstract class Date extends PluginBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $date_format = str_replace('ds_post_date_', '', $field['formatter']);
    return format_date($field['entity']->created->value, $date_format);
  }

  /**
   * {@inheritdoc}
   */
  public function formatters() {
    $date_types = \Drupal::entityManager()
      ->getStorageController('date_format')
      ->loadMultiple();

    $date_formatters = array();
    foreach ($date_types as $machine_name => $value) {
      if ($value->isLocked()) {
        continue;
      }
      $date_formatters['ds_post_date_' . $machine_name] = t($value->id());
    }

    return $date_formatters;
  }

}
