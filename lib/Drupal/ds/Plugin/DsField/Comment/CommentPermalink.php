<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Comment\CommentPermalink.
 */

namespace Drupal\ds\Plugin\DsField\Comment;

use Drupal\ds\Plugin\DsField\PreprocessBase;

/**
 * Plugin that renders the permalink of a comment.
 *
 * @DsField(
 *   id = "comment_permalink",
 *   title = @Translation("Permalink"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentPermalink extends PreprocessBase {

}
