<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\User.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders a view mode.
 *
 * @DsField(
 *   id = "user",
 *   title = @Translation("User"),
 *   entity_type = "node",
 *   provider = "user"
 * )
 */
class User extends Entity {

  /**
   * {@inhertidoc}
   */
  public function render() {
    $view_mode = $this->getViewMode();

    $node = $this->entity();
    $uid = $node->getAuthorId();

    $user = entity_load('user', $uid);
    $build = entity_view($user, $view_mode);

    return $build;
  }

  /**
   * {@inhertidoc}
   */
  public function linkedEntity() {
    return 'user';
  }

}
