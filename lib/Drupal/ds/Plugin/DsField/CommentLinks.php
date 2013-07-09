<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CommentLinks.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the administration links of the comment entity.
 *
 * @DsField(
 *   id = "comment_links",
 *   title = @Translation("Links"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentLinks extends PluginBase {

}
