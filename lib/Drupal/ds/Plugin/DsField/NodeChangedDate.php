<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeChangedDate.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders the post date of a node.
 *
 * @DsField(
 *   id = "node_changed_date",
 *   title = @Translation("Last modified"),
 *   entity_type = "node",
 *   module = "ds"
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
