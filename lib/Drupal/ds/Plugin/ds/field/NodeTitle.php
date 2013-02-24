
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
 *   id = "node_title",
 *   title = @Translation("Title"),
 *   entity_type = "node",
 *   module = "node"
 * )
 */
class NodeTitle extends Title {

}
