<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Controller\DsExtrasController.
 */

namespace Drupal\ds_extras\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Request;

/**
 * Returns responses for Display Suite Extra routes.
 */
class DsExtrasController extends ControllerBase {

  /**
   * Returns an node through JSON.
   *
   * @return array
   *   The Views fields report page.
   */
  public function switchViewModeInline(Request $request) {
    $content = '';
    $status = TRUE;
    $error = FALSE;

    $query = $request->query;
    $id = $query->get('id');
    $view_mode = $query->get('view_mode');
    $entity_type = $query->get('entity_type');
    $entity = entity_load($entity_type, $id);

    if (node_access('view', $entity)) {
      $element = entity_view($entity, $view_mode);
      $content = drupal_render($element);
    }
    else {
      $error = t('Access denied');
    }

    return new JsonResponse(array(
      'content' => $content,
      'status' => $status,
      'errorMessage' => $error
    ), 200);
  }

  /**
   * Renders the node
   */
  public function nodeView($node, Request $request) {

    $node = entity_load('node', $node);
    $uri = $node->uri();
    // Set the node path as the canonical URL to prevent duplicate content.
    drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
    // Set the non-aliased path as a default shortlink.
    drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);

    // Update the history table, stating that this user viewed this node.
    if ($this->moduleHandler()->moduleExists('history')) {
      history_write($node->id());
    }

    $view_mode = (!empty($node->get('ds_switch')->value)) ? $node->get('ds_switch')->value : 'full';

    // It's also possible to use a query argument named 'v' to switch view modes.
    $query_view_mode = $request->query->get('v');
    if (!empty($query_view_mode)) {
      $view_mode = $query_view_mode;
    }

    drupal_static('ds_extras_view_mode', $view_mode);
    return entity_view($node, $view_mode);
  }

}
