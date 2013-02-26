<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentPostDate.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the post date of a comment
 *
 * @Plugin(
 *   id = "node_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentPostDate extends Date {

}
