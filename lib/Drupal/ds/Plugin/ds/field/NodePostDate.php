
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\NodePostDate.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the post date of a node
 *
 * @Plugin(
 *   id = "node_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "node",
 *   module = "node"
 * )
 */
class NodePostDate extends Date {

}
