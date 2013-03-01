<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentUserSignature.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the user signature of a comment.
 *
 * @Plugin(
 *   id = "comment_user_signature",
 *   title = @Translation("User signature"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentUserSignature extends PreprocessPluginBase {

}
