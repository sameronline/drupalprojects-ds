<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\NodeSubmittedBy.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the submitted by field
 *
 * @DSPlugin(
 *   id = "node_submitted_by",
 *   title = @Translation("Submitted by"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeSubmittedBy extends Date {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\Date::renderField().
   */
  public function renderField($field) {
    $account = user_load($field['entity']->uid);
    switch ($field['formatter']) {
      case 'ds_time_ago':
        $interval = REQUEST_TIME - $field['entity']->created;
        return t('Submitted !interval ago by !user.', array('!interval' => format_interval($interval), '!user' => theme('username', array('account' => $account))));
      default:
        $date_format = str_replace('ds_post_date_', '', $field['formatter']);
        return t('Submitted by !user on !date.', array('!user' => theme('username', array('account' => $account)), '!date' => format_date($field['entity']->created, $date_format)));
    }
  }

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\Date::formatters().
   */
  public function formatters() {
    // Fetch all the date formatters
    $date_formatters = parent::formatters();

    // Add a "time ago" formatter
    $date_formatters['ds_time_ago'] = t('Time ago');

    return $date_formatters;
  }

}
