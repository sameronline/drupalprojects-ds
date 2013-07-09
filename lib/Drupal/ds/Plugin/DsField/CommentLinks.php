<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\CommentLinks.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the administration links of the comment entity.
 *
 * @DSPlugin(
 *   id = "comment_links",
 *   title = @Translation("Links"),
 *   entity_type = "comment",
 *   module = "ds"
 * )
 */
class CommentLinks extends PluginBase {

}
