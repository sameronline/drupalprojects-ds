<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentPermalink.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the permalink of a comment.
 *
 * @DSPlugin(
 *   id = "comment_permalink",
 *   title = @Translation("Permalink"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentPermalink extends PreprocessPluginBase {

}
