<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodePostDate.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders the post date of a node.
 *
 * @DsField(
 *   id = "node_post_date",
 *   title = @Translation("Post date"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodePostDate extends Date {

  /**
   * {@inheritdoc}
   */
  public function getRenderKey() {
    return 'created';
  }

}
