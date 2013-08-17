<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentTitle.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders the title of a comment.
 *
 * @DsField(
 *   id = "comment_title",
 *   title = @Translation("Title"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentTitle extends PreprocessPluginBase {

}
