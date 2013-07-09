<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentUserSignature.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the user signature of a comment.
 *
 * @DSPlugin(
 *   id = "comment_user_signature",
 *   title = @Translation("User signature"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentUserSignature extends PreprocessPluginBase {

}
