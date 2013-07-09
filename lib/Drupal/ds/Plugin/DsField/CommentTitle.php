<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentTitle.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the title of a comment.
 *
 * @DSPlugin(
 *   id = "comment_title",
 *   title = @Translation("Title"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentTitle extends PreprocessPluginBase {

}
