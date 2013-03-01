<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentAuthor.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the author of a comment.
 *
 * @Plugin(
 *   id = "comment_author",
 *   title = @Translation("Author"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentAuthor extends PreprocessPluginBase {

}
