<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentAuthor.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the author of a comment.
 *
 * @DsField(
 *   id = "comment_author",
 *   title = @Translation("Author"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentAuthor extends PreprocessPluginBase {

}
