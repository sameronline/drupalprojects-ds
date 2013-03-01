<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentSubmitted.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the submitted by element of a comment.
 *
 * @Plugin(
 *   id = "comment_submitted",
 *   title = @Translation("Submitted"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentSubmitted extends PreprocessPluginBase {

}
