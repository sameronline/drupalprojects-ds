<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentTitle.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a comment.
 *
 * @Plugin(
 *   id = "comment_title",
 *   title = @Translation("Title"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentTitle extends PreprocessPluginBase {

}
