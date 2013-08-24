<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Controller\DsExtrasController.
 */

namespace Drupal\ds_extras\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\JsonResponse;

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
  public function switchViewModeInline() {
    $content = '';
    $status = TRUE;
    $error = FALSE;

    $query = $this->request()->query;
    $id = $query->get('id');
    $view_mode = $query->get('view_mode');
    $entity_type = $query->get('entity_type');
    $entity = entity_load($entity_type, $id);

    if (node_access('view', $entity)) {
      $element = node_view($entity->getBCENtity(), $view_mode);
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

}
