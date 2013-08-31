<?php

/**
 * @file
 * Display Suite Extras page functions.
 */

/**
 * Menu callback: show an individual node with the Switch field.
 */
function ds_extras_node_page_view($node) {
  // If there is a menu link to this node, the link becomes the last part
  // of the active trail, and the link name becomes the page title.
  // Thus, we must explicitly set the page title to be the node title.
  drupal_set_title($node->title->value);
  $uri = $node->uri();
  // Set the node path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);

  // Update the history table, stating that this user viewed this node.
  if (Drupal::moduleHandler()->moduleExists('history')) {
    history_write($node->id());
  }

  // For markup consistency with other pages, use node_view_multiple() rather than node_view().
  $view_mode = (!empty($node->get('ds_switch')->value)) ? $node->get('ds_switch')->value : 'full';

  // It's also possible to use a query argument named 'v' to switch view modes.
  $query_view_mode = Drupal::request()->query->get('v');
  if (!empty($query_view_mode)) {
    $view_mode = $query_view_mode;
  }

  drupal_static('ds_extras_view_mode', $view_mode);
  return node_view_multiple(array($node->id() => $node), $view_mode);
}
