<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentAuthor.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders the author of a comment.
 *
 * @DsField(
 *   id = "comment_author",
 *   title = @Translation("Author"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentAuthor extends PreprocessBase {

}
