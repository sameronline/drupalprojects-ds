<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\NodeComments.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the comments of a node.
 *
 * @DsField(
 *   id = "node_comments",
 *   title = @Translation("Comments"),
 *   entity_type = "node",
 *   provider = "node"
 * )
 */
class NodeComments extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    if (in_array($view_mode, array('full', 'default'))) {
      return TRUE;
    }

    return FALSE;
  }

}
