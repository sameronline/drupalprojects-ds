<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentUserSignature.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the user signature of a comment.
 *
 * @DsField(
 *   id = "comment_user_signature",
 *   title = @Translation("User signature"),
 *   entity_type = "comment",
 *   provider = "comment"
 * )
 */
class CommentUserSignature extends PreprocessBase {

}
