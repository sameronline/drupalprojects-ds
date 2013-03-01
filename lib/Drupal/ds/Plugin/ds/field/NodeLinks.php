<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\NodeLinks.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the administration links of the node entity.
 *
 * @Plugin(
 *   id = "node_links",
 *   title = @Translation("Links"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeLinks extends PluginBase {

}
