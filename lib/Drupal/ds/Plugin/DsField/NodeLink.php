<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeTitle.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the title of a node
 *
 * @DsField(
 *   id = "node_link",
 *   title = @Translation("Read more"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeLink extends Link {

}
