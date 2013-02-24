
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\function_field\NodeTitle.
 */

namespace Drupal\ds\Plugin\ds\function_field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a node
 *
 * @Plugin(
 *   id = "node_link",
 *   title = @Translation("Read more"),
 *   entity_type = "node",
 *   module = "node"
 * )
 */
class NodeTitle extends Link {

}
