<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\function_field\NodeAuthor.
 */

namespace Drupal\ds\Plugin\ds\function_field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the author of a node
 *
 * @Plugin(
 *   id = "node_author",
 *   title = @Translation("Author"),
 *   entity_type = "node",
 *   module = "node"
 * )
 */
class NodeAuthor extends FunctionFieldPluginBase {

  /**
   * Implements \Drupal\ds\Plugin\ds\FieldPluginBase::renderField().
   */
  public function renderField($field) {
    // Users without a user name are anonymous users. These are never linked.
    if (empty($field['entity']->name)) {
      $anonymous_string = config('user.settings')->get('anonymous');
      return check_plain($anonymous_string);
    }

    if ($field['formatter'] == 'author') {
      return check_plain($field['entity']->name);
    }

    if ($field['formatter'] == 'author_linked') {
      return theme('username', array('account' => $field['entity']));
    }
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\FieldPluginBase::formatters().
   */
  public function formatters() {

   $formatters = array(
      'author' => t('Author'),
      'author_linked' => t('Author linked to profile')
    ),

    return $formatters;
  }

}
