<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeTitle.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the title of a node.
 *
 * @DsField(
 *   id = "node_title",
 *   title = @Translation("Title"),
 *   entity_type = "node",
 *   provider = "node"
 * )
 */
class NodeTitle extends Title {

}
