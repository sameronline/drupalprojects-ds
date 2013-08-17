<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeTitle.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders the title of a node.
 *
 * @DsField(
 *   id = "node_title",
 *   title = @Translation("Title"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeTitle extends Title {

}
