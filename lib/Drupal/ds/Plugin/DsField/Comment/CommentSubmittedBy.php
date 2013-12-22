<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Comment\CommentSubmittedBy.
 */

namespace Drupal\ds\Plugin\DsField\Comment;

use Drupal\ds\Plugin\DsField\PreprocessBase;

/**
 * Plugin that renders the submitted by element of a comment.
 *
 * @DsField(
 *   id = "comment_submitted",
 *   title = @Translation("Submitted by"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentSubmittedBy extends PreprocessBase {

}
