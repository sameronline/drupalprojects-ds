<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\NodeComments.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the comments of a node.
 *
 * @DSPlugin(
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
