<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\NodePostDate.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the post date of a node
 *
 * @DSPlugin(
 *   id = "node_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodePostDate extends Date {

}
