<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeChangedDate.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the post date of a node.
 *
 * @DsField(
 *   id = "node_changed_date",
 *   title = @Translation("Last modified"),
 *   entity_type = "node",
 *   provider = "node"
 * )
 */
class NodeChangedDate extends Date {

  /**
   * {@inheritdoc}
   */
  public function getRenderKey() {
    return 'changed';
  }

}
