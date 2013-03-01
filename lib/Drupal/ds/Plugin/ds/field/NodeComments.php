<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\NodeComments.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the comments of a node.
 *
 * @Plugin(
 *   id = "node_comments",
 *   title = @Translation("Comments"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeComments extends PluginBase {

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBase::displays().
   */
  public function displays() {
    $displays = array(
      '*|full',
      '*|default',
    )

    return $displays;
  }

}
