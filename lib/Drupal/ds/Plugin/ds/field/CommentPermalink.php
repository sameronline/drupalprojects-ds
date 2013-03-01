<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentPermalink.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the permalink of a comment.
 *
 * @Plugin(
 *   id = "comment_permalink",
 *   title = @Translation("Permalink"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentPermalink extends PreprocessPluginBase {

}
