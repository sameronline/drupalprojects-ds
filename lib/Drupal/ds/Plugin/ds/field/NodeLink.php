
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\NodeTitle.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a node
 *
 * @Plugin(
 *   id = "node_link",
 *   title = @Translation("Read more"),
 *   entity_type = "node",
 *   module = "node",
 *   field_type = "function"
 * )
 */
class NodeTitle extends Link {

}
