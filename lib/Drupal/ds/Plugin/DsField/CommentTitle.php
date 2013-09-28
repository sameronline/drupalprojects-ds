<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentTitle.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the title of a comment.
 *
 * @DsField(
 *   id = "comment_title",
 *   title = @Translation("Title"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentTitle extends Field {

  /**
   * @inheritdoc
   */
  protected function entityRenderKey() {
    return 'subject';
  }

}
