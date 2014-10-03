<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Controller\DsExtrasController.
 */

namespace Drupal\ds_extras\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\ds\Ds;
use Drupal\node\NodeInterface;
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

    if ($entity->access('view')) {
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
   * Checks access for the switch view mode route
   */
  public function accessSwitchViewMode(Request $request) {
    return ($this->currentUser()->hasPermission('access content') && $this->config('ds.extras')->get('switch_field') ? AccessInterface::ALLOW : AccessInterface::DENY);
  }

  /**
   * Displays a node revision.
   *
   * @param int $node_revision
   *   The node revision ID.
   *
   * @return array
   *   An array suitable for drupal_render().
   */
  public function revisionShow($node_revision) {
    $node = $this->entityManager()->getStorage('node')->loadRevision($node_revision);

    // Determine view mode.
    $view_mode = \Drupal::config('ds.extras')->get('override_node_revision_view_mode');
    drupal_static('ds_view_mode', $view_mode);

    $page =  node_view($node, $view_mode);
    unset($page['nodes'][$node->id()]['#cache']);

    return $page;
  }

}
