<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentPostDate.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the post date of a comment
 *
 * @DsField(
 *   id = "node_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentPostDate extends Date {

}
