<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentPostDate.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the post date of a comment.
 *
 * @DsField(
 *   id = "comment_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentPostDate extends Date {

}
