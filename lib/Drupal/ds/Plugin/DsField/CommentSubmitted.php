<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentSubmitted.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the submitted by element of a comment.
 *
 * @DSPlugin(
 *   id = "comment_submitted",
 *   title = @Translation("Submitted"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentSubmitted extends PreprocessPluginBase {

}
