<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\NodeTitle.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the title of a node
 *
 * @DSPlugin(
 *   id = "node_link",
 *   title = @Translation("Read more"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeLink extends Link {

}
