<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentPostDate.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the post date of a comment
 *
 * @DSPlugin(
 *   id = "node_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentPostDate extends Date {

}
