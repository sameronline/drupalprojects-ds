<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentChangedDate.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the changed date of a comment.
 *
 * @DsField(
 *   id = "comment_changed_date",
 *   title = @Translation("Last modified"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentChangedDate extends Date {

  /**
   * {@inheritdoc}
   */
  public function getRenderKey() {
    return 'changed';
  }

}
