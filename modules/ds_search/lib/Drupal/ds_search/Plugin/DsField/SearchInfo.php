<?php

/**
 * @file
 * Contains \Drupal\ds_search\Plugin\DsField\SearchInfo.
 */

namespace Drupal\ds_search\Plugin\DsField;

use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Plugin that prints the search info
 *
 * @DsField(
 *   id = "search_info",
 *   title = @Translation("Search info"),
 *   entity_type = "node"
 * )
 */
class SearchInfo extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $node = $this->entity();

    if (!empty($node->search_extra)) {
      $info['user'] = theme('username', array('account' => $node->getAuthor()));
      $info['date'] = format_date($node->changed->value, 'short');
      if (isset($node->search_extra) && is_array($node->search_extra)) {
        $info = array_merge($info, $node->search_extra);
      }

      return array(
        '#markup' => implode(' - ', $info),
      );
    }

    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function isAllowed() {
    $search_view_mode = \Drupal::config('ds_search.settings')->get('view_mode');
    return $this->viewMode() == $search_view_mode;
  }

}
