<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeComments.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the comments of a node.
 *
 * @DsField(
 *   id = "node_comments",
 *   title = @Translation("Comments"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class NodeComments extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function displays() {
    $displays = array(
      '*|full',
      '*|default',
    );

    return $displays;
  }

}
