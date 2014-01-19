<?php
/**
 * @file
 * Display Suite revision callback.
 */

use Drupal\ds\Ds;

/**
 * Menu callback: show an individual revision node using the revision view mode.
 */
function ds_revision_node_show($node, $message = NULL) {

  drupal_set_title(t('Revision of %title from %date', array('%title' => $node->title, '%date' => format_date($node->revision_timestamp))), PASS_THROUGH);

  // Determine view mode.
  $layout = Ds::getLayout('node', $node->type, 'revision', FALSE);
  $view_mode = ($layout) ? 'revision' : 'full';
  drupal_static('ds_view_mode', $view_mode);

  // For markup consistency with other pages, use node_view_multiple() rather than node_view().
  return node_view_multiple(array($node->nid => $node), $view_mode);
}
