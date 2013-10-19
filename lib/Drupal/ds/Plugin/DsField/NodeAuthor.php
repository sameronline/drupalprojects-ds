<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeAuthor.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the author of a node.
 *
 * @DsField(
 *   id = "node_author",
 *   title = @Translation("Author"),
 *   entity_type = "node",
 *   provider = "node"
 * )
 */
class NodeAuthor extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $user = $this->entity->getAuthor();

    // Users without a user name are anonymous users. These are never linked.
    if (empty($user->name)) {
      $anonymous_string = \Drupal::config('user.settings')->get('anonymous');
      return array(
        '#markup' => check_plain($anonymous_string),
      );
    }

    $field = $this->getFieldConfiguration();
    if ($field['formatter'] == 'author') {
      return array(
        '#markup' => $user->getUsername(),
      );
    }

    if ($field['formatter'] == 'author_linked') {
      return array(
        '#markup' => theme('username', array('account' => $user)),
      );
    }
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
