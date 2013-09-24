<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentSubmitted.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the submitted by element of a comment.
 *
 * @DsField(
 *   id = "comment_submitted",
 *   title = @Translation("Submitted"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentSubmitted extends PreprocessBase {

}
