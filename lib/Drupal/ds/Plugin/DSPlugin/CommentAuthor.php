<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentAuthor.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the author of a comment.
 *
 * @DSPlugin(
 *   id = "comment_author",
 *   title = @Translation("Author"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentAuthor extends PreprocessPluginBase {

}
