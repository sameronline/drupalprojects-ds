<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeAuthor.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the author of a node
 *
 * @DsField(
 *   id = "node_author",
 *   title = @Translation("Author"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeAuthor extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {

    // Users without a user name are anonymous users. These are never linked.
    if (empty($field['entity']->name)) {
      $anonymous_string = config('user.settings')->get('anonymous');
      return check_plain($anonymous_string);
    }

    if ($field['formatter'] == 'author') {
      return user_format_name($field['entity']);
    }

    if ($field['formatter'] == 'author_linked') {
      return theme('username', array('account' => $field['entity']));
    }

    // Formatter handling isn't working yet
    // Todo remove this once formatters are saved again.
    return check_plain($field['entity']->name);
  }

  /**
   * {@inheritdoc}
   */
  public function formatters() {

    $formatters = array(
      'author' => t('Author'),
      'author_linked' => t('Author linked to profile')
    );

    return $formatters;
  }

}
