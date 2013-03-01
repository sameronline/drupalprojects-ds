<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CommentLinks.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the administration links of the comment entity.
 *
 * @Plugin(
 *   id = "comment_links",
 *   title = @Translation("Links"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentLinks extends PluginBase {

}
