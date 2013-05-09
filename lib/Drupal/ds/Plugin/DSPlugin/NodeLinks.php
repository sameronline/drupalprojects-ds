<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\NodeLinks.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the administration links of the node entity.
 *
 * @DSPlugin(
 *   id = "node_links",
 *   title = @Translation("Links"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeLinks extends PluginBase {

}
