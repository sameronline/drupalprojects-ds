<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Comment\CommentLinks.
 */

namespace Drupal\ds\Plugin\DsField\Comment;

use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Plugin that renders the administration links of the comment entity.
 *
 * @DsField(
 *   id = "comment_links",
 *   title = @Translation("Links"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentLinks extends DsFieldBase {

}
